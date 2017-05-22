#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <linux/spinlock_types.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>


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

    switch(cmd)
    {
	default: break;
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

    if (misc_register(&shadow_miscdev))
    {
       printk(KERN_ERR "cannot register miscdev on minor=%d\n", 10);
       return -1;
    }
   
    printk(KERN_INFO"registered miscdev on minor=%d\n", shadow_miscdev.minor);


    printk(KERN_INFO"Shadow module initialized\n");

    return 0;
}
static void __exit shadow_lbr_exit(void) {
    unsigned long flags;


    misc_deregister(&shadow_miscdev);
   
    printk(KERN_INFO"Shadow module removed\n");
}

module_init(shadow_lbr_init);
module_exit(shadow_lbr_exit);

