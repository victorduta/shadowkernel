#ifndef __WRAPER_H__
#define __WRAPER_H__

//#include <linux/spinlock_types.h>

/* Controls insertion-deinsertion of the lbr module */
#define SHADOW_MODULE_ON 2
#define SHADOW_MODULE_PHASE_2 1
#define SHADOW_MODULE_OFF 0

extern int shadow_state;

typedef void (*check_lbr_func_t)(void *addr);

extern check_lbr_func_t check_wrapper;
extern check_lbr_func_t check_lbr;
//extern spinlock_t check_lbr_lock;


void check_wrapper_default(void *addr);
void check_wrapper_init(void *addr);
void check_lbr_default(void *addr);

#endif



