#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <linux/spinlock_types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include "shadow.h"
#include "wrapper.h"
#include "lbr-state.h"
#include "cache.h"

MODULE_AUTHOR("Duta Victor Marin");
MODULE_DESCRIPTION("Release Shadow LBR Module");
MODULE_LICENSE("GPL");

#define CHUNK_SIZE 1000

/****************************************************************************
 * EXPORTED FROM WRAPPER.C                                                  
 */

extern struct address_entry lbr_entry_table[NUM_UNITS][NUM_ENTRIES];

/****************************************************************************
 * MODULE FUNCTIONS AND VARS
 */

static int shadow_users = 0;
static int shadow_initialized = 0;
static struct lbr_stats stats;
static struct function_stats f_stats;
static struct address_entry f_entries[NUM_ENTRIES];

noinline void test_rdmsrl()
{
   struct lbr_entry lbr;
   preempt_disable(); 
   rdmsrl(MSR_LBR_TOS,  lbr.tos);
   rdmsrl(MSR_LBR_NHM_FROM + lbr.tos, lbr.from);
   preempt_enable();
}

void get_lbr_stats(void *info) {
    unsigned long flags;
    struct lbr_stats *stats=(struct lbr_stats *)(info);
    spin_lock_irqsave(&(stats->lock), flags);
    
    stats->n_hits += get_cpu_var(n_hits);
    put_cpu_var(n_hits);
    stats->n_misses += get_cpu_var(n_misses);
    put_cpu_var(n_misses);

    spin_unlock_irqrestore(&(stats->lock), flags);
}

void init_function_stats(void *info)
{
   int cpu;
   struct address_entry *lbr_entries;
   cpu = get_cpu();
   lbr_entries = lbr_entry_table[cpu];
   memset(lbr_entries, 0, NUM_ENTRIES*sizeof(struct address_entry));
   put_cpu();
}
#ifndef INCLUDE_EXTENDED_MEASUREMENTS
void get_function_stats(void *info)
{
   struct address_entry *lbr_entries;
   unsigned long flags;
   int i, j, cpu;
   struct function_stats *stats=(struct function_stats *)(info);   
   cpu = get_cpu();
   lbr_entries = lbr_entry_table[cpu];
   spin_lock_irqsave(&(stats->lock), flags);
   for(i = 0; i < NUM_ENTRIES; i++)
   {
      if (lbr_entries[i].to == 0)
      {
         break;
      }
      for(j = 0; j < NUM_ENTRIES; j++)
      {
           if(stats->entries[j].to == lbr_entries[i].to)
           {
                stats->entries[j].nhits += lbr_entries[i].nhits;
                break;
           }
           if(stats->entries[j].to == 0)
           {
              stats->entries[j].tos =  lbr_entries[i].tos;
              stats->entries[j].from = lbr_entries[i].from;
              stats->entries[j].to =   lbr_entries[i].to;
              stats->entries[j].address = lbr_entries[i].address;
              stats->entries[j].nhits = lbr_entries[i].nhits;
              break;
           }
      }
   }
   spin_unlock_irqrestore(&(stats->lock), flags);
   put_cpu();
}
#else
void get_function_stats(void *info)
{
   struct address_entry *lbr_entries;
   unsigned long flags;
   int i, j, cpu, k, t;
   struct function_stats *stats=(struct function_stats *)(info);   
   cpu = get_cpu();
   lbr_entries = lbr_entry_table[cpu];
   spin_lock_irqsave(&(stats->lock), flags);
   for(i = 0; i < NUM_ENTRIES; i++)
   {
      if (lbr_entries[i].to == 0)
      {
         break;
      }
      for(j = 0; j < NUM_ENTRIES; j++)
      {
           if(stats->entries[j].to == lbr_entries[i].to)
           {
                for( k = 0; k < lbr_entries[i].from_length; k++)
                {
                   for(t = 0; t < stats->entries[j].from_length; t++)
                   {
                       if (lbr_entries[i].from[k].from == stats->entries[j].from[t].from)
                       {
                           stats->entries[j].from[t].nhits += lbr_entries[i].from[k].nhits;
                           break;
                       }
                   } 
 
                   if ( (t ==  stats->entries[j].from_length) && (stats->entries[j].from_length <= NUM_FROM))
                   {
                        stats->entries[j].from_length++;
                        stats->entries[j].from[t].nhits = lbr_entries[i].from[k].nhits;
                        stats->entries[j].from[t].from =  lbr_entries[i].from[k].from;
                   }
                }
                stats->entries[j].nhits += lbr_entries[i].nhits;
                break;
           }
           if(stats->entries[j].to == 0)
           {
              stats->entries[j].tos =  lbr_entries[i].tos;
              stats->entries[j].to =   lbr_entries[i].to;
              stats->entries[j].address = lbr_entries[i].address;
              stats->entries[j].nhits = lbr_entries[i].nhits;
              stats->entries[j].from_length = lbr_entries[i].from_length;
              memcpy(stats->entries[j].from, lbr_entries[i].from,  (lbr_entries[i].from_length)*sizeof(struct from_entry));
              break;
           }
      }
   }
   spin_unlock_irqrestore(&(stats->lock), flags);
   put_cpu();
}
#endif

/*****************************************************************************
 * MODULE INTERFACE
 */

int shadow_open(struct inode *inode, struct file *filp) {
    shadow_users++;

    if(shadow_initialized) 
                 return 0;
    
    shadow_initialized = 1;
    return 0;
}

int shadow_close(struct inode *inode, struct file *filp) {
    shadow_users--;

    if(shadow_users) 
           return 0;

    shadow_initialized = 0;
    return 0;
}

static long shadow_ioctl(struct file *file, unsigned int cmd, unsigned long arg1) {
    static int n_sent = 0;
    static int initialized = 0;
    unsigned long flags;
    struct lbr_entry lbr;
    uint32_t cycles_high_start, cycles_low_start, cycles_high_finish, cycles_low_finish;
    unsigned long long address = CACHE_SIZE+1;
    int i;
    int wait = 1;
    switch(cmd)
    {
	case SHADOW_IOC_TEST:
                           
                           stats.n_misses = 0;
                           stats.n_hits = 0;
                           spin_lock_init(&(stats.lock));
                           on_each_cpu(get_lbr_stats, &stats, wait);

                           printk(KERN_INFO "Shadow LBR ioctl lbr stats\n");
                           printk(KERN_INFO "N_HITS:%lld\n", stats.n_hits);
                           printk(KERN_INFO "N_MISSES:%lld\n", stats.n_misses);

                           break;
        case SHADOW_IOC_GET_CYCLES:
			   __asm__ __volatile__("mfence;"
		                                "CPUID\n\t"/*serialize*/
                                                "RDTSC\n\t"/*read the clock*/
                                                "mov %%edx, %0\n\t"
                                                "mov %%eax, %1\n\t": "=r" (cycles_high_start), "=r"
                                                (cycles_low_start):: "%rax", "%rbx", "%rcx", "%rdx");
                            preempt_disable(); 
                            rdmsrl(MSR_LBR_TOS,  lbr.tos);
                            rdmsrl(MSR_LBR_NHM_FROM + lbr.tos, lbr.from);
                            preempt_enable();
                            __asm__ __volatile__ ("RDTSCP\n\t"/*read the clock*/
                                                  "mov %%edx, %0\n\t"
                                                  "mov %%eax, %1\n\t"
                                                  "CPUID\n\t"
                                                  "mfence;": "=r" (cycles_high_finish), "=r"
                                                  (cycles_low_finish):: "%rax", "%rbx", "%rcx", "%rdx");

                           printk(KERN_INFO "RDMSR cycles %d\n", cycles_low_finish - cycles_low_start);
                           init_dummy_cache();
                           __asm__ __volatile__("mfence;"
		                                "CPUID\n\t"/*serialize*/
                                                "RDTSC\n\t"/*read the clock*/
                                                "mov %%edx, %0\n\t"
                                                "mov %%eax, %1\n\t": "=r" (cycles_high_start), "=r"
                                                (cycles_low_start):: "%rax", "%rbx", "%rcx", "%rdx");
                           get_entry(address);
                           //add_entry(address, 0);
                            __asm__ __volatile__ ("RDTSCP\n\t"/*read the clock*/
                                                  "mov %%edx, %0\n\t"
                                                  "mov %%eax, %1\n\t"
                                                  "CPUID\n\t"
                                                  "mfence;": "=r" (cycles_high_finish), "=r"
                                                  (cycles_low_finish):: "%rax", "%rbx", "%rcx", "%rdx");

                           printk(KERN_INFO "Cache add cycles %d\n", cycles_low_finish - cycles_low_start);

                           __asm__ __volatile__("mfence;"
		                                "CPUID\n\t"/*serialize*/
                                                "RDTSC\n\t"/*read the clock*/
                                                "mov %%edx, %0\n\t"
                                                "mov %%eax, %1\n\t": "=r" (cycles_high_start), "=r"
                                                (cycles_low_start):: "%rax", "%rbx", "%rcx", "%rdx");
                           test_rdmsrl();
                            __asm__ __volatile__ ("RDTSCP\n\t"/*read the clock*/
                                                  "mov %%edx, %0\n\t"
                                                  "mov %%eax, %1\n\t"
                                                  "CPUID\n\t"
                                                  "mfence;": "=r" (cycles_high_finish), "=r"
                                                  (cycles_low_finish):: "%rax", "%rbx", "%rcx", "%rdx");

                           printk(KERN_INFO "Test rdmsrl1 %d\n", cycles_low_finish - cycles_low_start);

                           __asm__ __volatile__("mfence;"
		                                "CPUID\n\t"/*serialize*/
                                                "RDTSC\n\t"/*read the clock*/
                                                "mov %%edx, %0\n\t"
                                                "mov %%eax, %1\n\t": "=r" (cycles_high_start), "=r"
                                                (cycles_low_start):: "%rax", "%rbx", "%rcx", "%rdx");
                           test_rdmsrl();
                            __asm__ __volatile__ ("RDTSCP\n\t"/*read the clock*/
                                                  "mov %%edx, %0\n\t"
                                                  "mov %%eax, %1\n\t"
                                                  "CPUID\n\t"
                                                  "mfence;": "=r" (cycles_high_finish), "=r"
                                                  (cycles_low_finish):: "%rax", "%rbx", "%rcx", "%rdx");

                           printk(KERN_INFO "Test rdmsrl2 %d\n", cycles_low_finish - cycles_low_start);
                          
			   break;
         case SHADOW_IOC_INIT_ENTRIES:
			   on_each_cpu(init_function_stats, NULL, wait);
                           break;
         case SHADOW_IOC_GET_ENTRIES:
                           if (!initialized)
                           {
                             spin_lock_init(&(f_stats.lock));
                             f_stats.entries = f_entries;
                             memset(f_entries, 0, NUM_ENTRIES*sizeof(struct address_entry));
                             on_each_cpu(get_function_stats, &f_stats, wait);
                             initialized = 1;
                             n_sent = 0;
                             printk(KERN_INFO"Reinit record structure\n");
                           }
                           if (n_sent >= NUM_ENTRIES)
                           {
                               initialized = 0;
                               n_sent = 0;
                               return 0;
                           } 
                           if (copy_to_user((void __user *)arg1, &f_entries[n_sent], CHUNK_SIZE*sizeof(struct address_entry)) != 0) {
                                               printk(KERN_ERR "Could not copy needed records to userland\n");
                                               initialized = 0;
                                               n_sent = 0;
                                               return -1;
                           }
                           n_sent += CHUNK_SIZE;
                           return CHUNK_SIZE;
         
    }
    return 0;

}


/******************************************************************************
 * MODULE SETUP                                                               
 */

static struct file_operations shadow_fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = shadow_ioctl,
    .open           = shadow_open,
    .release        = shadow_close,
};

static struct miscdevice shadow_miscdev = {
    .minor          = MISC_DYNAMIC_MINOR,
    .name           = "shadow_lbr",
    .fops           = &shadow_fops,
}; 



static int __init shadow_lbr_init(void) {
    unsigned long flags;
    int wait = 1;
    struct lbr_t lbr;

    if (misc_register(&shadow_miscdev))
    {
       printk(KERN_ERR "cannot register miscdev on minor=%d\n", SHADOW_MINOR);
       return -1;
    }
   
    printk(KERN_INFO"registered miscdev on minor=%d\n", shadow_miscdev.minor);

    /* Enable lbr on each cpu */
    on_each_cpu(enable_lbr, NULL, wait);
    
    get_cpu();
    get_lbr(&lbr);
    dump_lbr(&lbr);
    put_cpu();


    printk(KERN_INFO"Shadow module initialized\n");

    return 0;
}
static void __exit shadow_lbr_exit(void) {
    unsigned long flags;
    int wait = 1;

    /* Disable lbr on each cpu */
    on_each_cpu(disable_lbr, NULL, wait);

    misc_deregister(&shadow_miscdev);

   
    printk(KERN_INFO"Shadow module removed\n");
}

module_init(shadow_lbr_init);
module_exit(shadow_lbr_exit);

