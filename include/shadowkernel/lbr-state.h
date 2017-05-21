#ifndef LBR_STATE_H
#define LBR_STATE_H

#ifndef __KERNEL__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif 

#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <stdint.h>
#include <stdio.h>

#endif /* __KERNEL__ */


/* This one is also defined in lbr.h, but lbr.h is not included (as it is only
 * kernel stuff there). 
 */
#define LBR_ENTRIES 16


struct lbr_t {
    uint64_t debug;   // contents of IA32_DEBUGCTL MSR
    uint64_t select;  // contents of LBR_SELECT
    uint64_t tos;     // index to most recent branch entry
    uint64_t from[LBR_ENTRIES];
    uint64_t   to[LBR_ENTRIES];
    struct task_struct *task; // pointer to the task_struct this state belongs to
};

/* This struct might be better in case we use it in userspace */
struct lbr_record_t {
    uint64_t debug;   // contents of IA32_DEBUGCTL MSR
    uint64_t select;  // contents of LBR_SELECT
    uint64_t tos;     // index to most recent branch entry
    uint64_t from[LBR_ENTRIES];
    uint64_t   to[LBR_ENTRIES];
};


#ifndef __KERNEL__
static inline void dump_lbr_record(struct lbr_t *lbr)
{
    int i;
    printf("TASK:                 0x%p\n", lbr->task);
    printf("MSR_IA32_DEBUGCTLMSR: 0x%llx\n", lbr->debug);
    printf("MSR_LBR_SELECT:       0x%llx\n", lbr->select);
    printf("MSR_LBR_TOS:          %lld\n", lbr->tos);
    for (i = 0; i < LBR_ENTRIES; i++) {
      printf("MSR_LBR_NHM_FROM[%2d]: 0x%llx\n", i, lbr->from[i]);
      printf("MSR_LBR_NHM_TO  [%2d]: 0x%llx\n", i, lbr->to[i]);
    }

}
#endif


#endif /* __LBR_STATE__ */


