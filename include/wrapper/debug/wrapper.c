
#include "wrapper.h"
/* Keeps track if the shadow module is initialized or not */
int shadow_state = SHADOW_MODULE_OFF;

EXPORT_SYMBOL(shadow_state);

DEFINE_SPINLOCK(check_lbr_lock);

EXPORT_SYMBOL(check_lbr_lock);

check_lbr_func_t check_lbr = check_lbr_default;

EXPORT_SYMBOL(check_lbr);

check_lbr_func_t check_wrapper = check_wrapper_default;

EXPORT_SYMBOL(check_wrapper);

int lbr_test_var = 0;

EXPORT_SYMBOL(lbr_test_var);


void check_lbr_default(void *addr)
{
     return; // Do nothing
}

EXPORT_SYMBOL(check_lbr_default);

void check_wrapper_default(void *addr)
{
    unsigned long flags;
    
    spin_lock_irqsave(&check_lbr_lock, flags);

    if(shadow_state == SHADOW_MODULE_PHASE_2)
    {
       check_wrapper = check_wrapper_init;
       shadow_state = SHADOW_MODULE_ON;
       lbr_test_var = 2;
    }
    lbr_test_var = 1;
    spin_unlock_irqrestore(&check_lbr_lock, flags);
    
}

void check_wrapper_init(void *addr)
{
     unsigned long flags;
     check_lbr_func_t checker_f;

     spin_lock_irqsave(&check_lbr_lock, flags);

     /* Make sure that we don't get a race condition while we call the check_lbr
        while someone changes the check_lbr pointer. This is done within the 
        check_lbr_lock so taking the value while the lock is down is safe */

     checker_f = check_lbr;

     if (shadow_state == SHADOW_MODULE_PHASE_2)
     {
        check_wrapper = check_wrapper_default;
        shadow_state = SHADOW_MODULE_OFF;
        spin_unlock_irqrestore(&check_lbr_lock, flags);
        return;
     }

     spin_unlock_irqrestore(&check_lbr_lock, flags);

     checker_f(addr); // Our checking function provided by the module
     
}


