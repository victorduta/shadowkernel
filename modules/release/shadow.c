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

MODULE_AUTHOR("Duta Victor Marin");
MODULE_DESCRIPTION("Release Shadow LBR Module");
MODULE_LICENSE("GPL");


/****************************************************************************
 * EXPORTED FROM WRAPPER.C                                                  
 */


/****************************************************************************
 * MODULE FUNCTIONS AND VARS
 */

static int shadow_users = 0;
static int shadow_initialized = 0;
static struct lbr_stats stats;

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
    //on_each_cpu(enable_lbr, NULL, wait);


    printk(KERN_INFO"Shadow module initialized\n");

    return 0;
}
static void __exit shadow_lbr_exit(void) {
    unsigned long flags;
    int wait = 1;

    /* Disable lbr on each cpu */
    //on_each_cpu(disable_lbr, NULL, wait);

    misc_deregister(&shadow_miscdev);

   
    printk(KERN_INFO"Shadow module removed\n");
}

module_init(shadow_lbr_init);
module_exit(shadow_lbr_exit);

