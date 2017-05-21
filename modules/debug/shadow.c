#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <linux/spinlock_types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include "lbr.h"
#include "lbr-state.h"
#include "shadow.h"
#include "wrapper.h"

#define MAX_OFFSET 8
#define LBR_ENTRY_OFFSET 2

MODULE_AUTHOR("Duta Victor Marin");
MODULE_DESCRIPTION("Shadow LBR Module");
MODULE_LICENSE("GPL");


/****************************************************************************
 * EXPORTED FROM WRAPPER.C                                                  
 */


/* TODO find a way to include check_lbr_lock in a header */
extern spinlock_t check_lbr_lock;
extern int lbr_test_var;

/****************************************************************************
 * MODULE FUNCTIONS AND VARS
 */

static int shadow_users = 0;
static int shadow_initialized = 0;
/* used for recording branches */
static int records_needed = 0;
static int n_recorded = 0;
static struct shadow_record_t *record_list = NULL;
static spinlock_t record_lock;
static spinlock_t rewrite_lock;

/* used in lbr_rewrite_hook */
static uint64_t n_misses = 0;
static uint64_t n_hits = 0;
static uint64_t n_pad_misses = 0;

/* These call sites don't get instrumented */
static int not_padded_size = 7;
static uint64_t not_padded[7] = {0xffffffff8104c0d0, 0xffffffff8105ea6c, 0xffffffff8105eb50, 0xffffffff8105ec72, 0xffffffff8105ee9d, 0xffffffff810602cd, 0xffffffff810602ed };
static int pad_mask[7] = {0,0,0,0,0,0,0};

/****************************************************************************
 * HOOKS FOR RETURN ADDRESS VERIFICATION                              
 */

void install_lbr_hook(check_lbr_func_t hook)
{
     unsigned long flags;
     spin_lock_irqsave(&check_lbr_lock, flags);
     check_lbr = hook;
     spin_unlock_irqrestore(&check_lbr_lock, flags);
}

void check_lbr_hook(void *addr)
{
    lbr_test_var = 3;
}

void lbr_rewrite_hook(void *ret_addr)
{
     struct shadow_record_t record;
     uint64_t poz;
     int i;
     unsigned long flags;
     uint64_t  from;
     uint64_t* addr = (uint64_t*)((char *)ret_addr+8); // Maby it is better to set addr as uint64_t
     
     get_cpu();
     get_lbr_record(&record.lbr_state);
     put_cpu();

     poz = (((record.lbr_state.tos - LBR_ENTRY_OFFSET) + 16) % 16);
     from =  record.lbr_state.from[poz];
     if((*addr - from) > MAX_OFFSET)
     {
         spin_lock_irqsave(&rewrite_lock, flags);
         n_misses++; 
         spin_unlock_irqrestore(&rewrite_lock, flags);
         return;
     }
     else
     {
        for(i=0; i < not_padded_size; i++)
        {
           if(from == not_padded[i])
           {
             spin_lock_irqsave(&rewrite_lock, flags);
             pad_mask[i] = 1;
             n_pad_misses++;
             spin_unlock_irqrestore(&rewrite_lock, flags);
             return;
           }
        }
        spin_lock_irqsave(&rewrite_lock, flags);
        n_hits++;
        spin_unlock_irqrestore(&rewrite_lock, flags);

        //*addr = from+MAX_OFFSET;
     }
     
     
}

void lbr_record_hook(void *addr)
{
     unsigned long flags;
     spin_lock_irqsave(&record_lock, flags);
     if(n_recorded == records_needed)
     {
        spin_unlock_irqrestore(&record_lock, flags);
        /* Uninstall the hook as we don't need it anymore 
           There might be a chance that multiple threads
           execute this but as this is atomic it won't
           be a problem */
        install_lbr_hook(lbr_rewrite_hook);
        return;
     }
     record_list[n_recorded].addr = *((uint64_t*)((char *)addr+8));
     record_list[n_recorded].cpu = get_cpu();
     get_lbr_record(&record_list[n_recorded].lbr_state);
     put_cpu();
     n_recorded++;
     lbr_test_var = 5;
     spin_unlock_irqrestore(&record_lock, flags);
}


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
    unsigned long flags;
    int to_send;
    static int records_sent = 0;
    int i;

    switch(cmd)
    {
       case SHADOW_IOC_TEST:
                              printk(KERN_INFO "Shadow LBR ioctl system works %d\n", lbr_test_var);
			      printk(KERN_INFO "Misses %lld\n", n_misses);
                              printk(KERN_INFO "Hits %lld\n", n_hits);
                              printk(KERN_INFO "Unwanted cases %lld\n", n_pad_misses);
                              for(i = 0; i < 7; i++)
                              {
                                 printk(KERN_INFO "Hit %d %d\n",i, pad_mask[i]);
                              }
			      break;
       case SHADOW_IOC_INSTALL_DEFAULT_HOOK:
                              install_lbr_hook(check_lbr_hook);
                              break;
       case SHADOW_IOC_START_RECORDING:
                              if(records_needed != 0)
                              {
                                  printk(KERN_INFO "Another record request on the way\n");
                                  return -1;
                              }
                              if (copy_from_user(&records_needed, (void __user *)arg1, sizeof(int)) != 0) {
                                               printk(KERN_ERR "Could not copy needed record variable from userland\n");
                                               records_needed = 0;
                                               return 1;
                              }
                              printk(KERN_INFO "Allocating space for %d records\n", records_needed);
                              if ((record_list = kmalloc(records_needed* sizeof(struct shadow_record_t), GFP_KERNEL)) == NULL)
                              {
                                               printk(KERN_ERR "Could not allocate memory for record_list\n");
                                               records_needed = 0;
                                               return -1;
                              }
                              /* Initialize the number of records copied back to userspace */
                              records_sent = 0;
                              n_recorded = 0;

                              /* Initialize the lock used to make recording atomic */
                              spin_lock_init(&record_lock);

                              /* Add hook for recording return addressess and lbr state */
                              install_lbr_hook(lbr_record_hook);
                              
                              break;

      case SHADOW_IOC_GET_RECORDS:
                              if (!records_needed) /* Notify userland that no more records are to be sent */
                                  return -1;
                              spin_lock_irqsave(&record_lock, flags);
                              to_send = n_recorded - records_sent;
                              if(n_recorded == records_needed)
                              {
                                  records_needed = 0;
                                  n_recorded = 0;
                              }
                              spin_unlock_irqrestore(&record_lock, flags);
                              if (copy_to_user((void __user *)arg1, &record_list[records_sent], to_send*sizeof(struct shadow_record_t)) != 0) {
                                               printk(KERN_ERR "Could not copy needed records to userland\n");
                                               records_needed = 0;
                                               return -1;
                              }
                              records_sent+=to_send;

                              return to_send; 
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

    if (misc_register(&shadow_miscdev))
    {
       printk(KERN_ERR "cannot register miscdev on minor=%d\n", SHADOW_MINOR);
       return -1;
    }
   
    printk(KERN_INFO"registered miscdev on minor=%d\n", shadow_miscdev.minor);

    /* Enable lbr on each cpu */
    on_each_cpu(enable_lbr, NULL, wait);

    /* Enable lock for the rewrite hook*/
    spin_lock_init(&rewrite_lock);
    
    spin_lock_irqsave(&check_lbr_lock, flags);
    
    check_lbr = lbr_rewrite_hook;
    shadow_state = SHADOW_MODULE_PHASE_2;

    spin_unlock_irqrestore(&check_lbr_lock, flags);

    printk(KERN_INFO"Shadow module initialized\n");

    return 0;
}
static void __exit shadow_lbr_exit(void) {
    unsigned long flags;
    int wait = 1;

    spin_lock_irqsave(&check_lbr_lock, flags);
    
    check_lbr = check_lbr_default;
    shadow_state = SHADOW_MODULE_PHASE_2;

    spin_unlock_irqrestore(&check_lbr_lock, flags);

    on_each_cpu(disable_lbr, NULL, wait);

    misc_deregister(&shadow_miscdev);
   
    printk(KERN_INFO"Shadow module removed\n");
}

module_init(shadow_lbr_init);
module_exit(shadow_lbr_exit);

