

#include "lbr.h"
#include "lbr-state.h"


#ifdef SHADOW_DEBUG
inline void printd(bool print_cpuid, const char *fmt, ...) {
    int core_id, thread_id;
    va_list args;

    //if (print_cpuid) cpuids(&core_id, &thread_id);

    va_start(args, fmt);
    //if (print_cpuid) printk("A:%d:%d: ", core_id, thread_id);
    vprintk(fmt, args);
    va_end(args);
}
#else
inline void printd(bool print_cpuid, const char *fmt, ...) {
}
#endif


/***********************************************************************
 * Helper functions for LBR related functionality.
 */

/* Flush the LBR registers. Caller should do get_cpu() and put_cpu().  */
void flush_lbr(bool enable) {
    int i;

    wrmsrl(MSR_LBR_TOS, 0);
    for (i = 0; i < LBR_ENTRIES; i++) {
        wrmsrl(MSR_LBR_NHM_FROM + i, 0);
        wrmsrl(MSR_LBR_NHM_TO   + i, 0);
    }
    if (enable) wrmsrl(MSR_IA32_DEBUGCTLMSR, IA32_DEBUGCTL);
    else        wrmsrl(MSR_IA32_DEBUGCTLMSR, 0);
}

/* Store the LBR registers for the current CPU into <lbr>. */
void get_lbr(struct lbr_t *lbr) {
    int i;

    rdmsrl(MSR_IA32_DEBUGCTLMSR, lbr->debug);
    rdmsrl(MSR_LBR_SELECT,       lbr->select);
    rdmsrl(MSR_LBR_TOS,          lbr->tos);
    for (i = 0; i < LBR_ENTRIES; i++) {
        rdmsrl(MSR_LBR_NHM_FROM + i, lbr->from[i]);
        rdmsrl(MSR_LBR_NHM_TO   + i, lbr->to  [i]);

        lbr->from[i] = LBR_FROM(lbr->from[i]);
    }
}

/* Write the LBR registers for the current CPU. */
void put_lbr(struct lbr_t *lbr) {
    int i;

    wrmsrl(MSR_IA32_DEBUGCTLMSR, lbr->debug);
    wrmsrl(MSR_LBR_SELECT,       lbr->select);
    wrmsrl(MSR_LBR_TOS,          lbr->tos);
    for (i = 0; i < LBR_ENTRIES; i++) {
        wrmsrl(MSR_LBR_NHM_FROM + i, lbr->from[i]);
        wrmsrl(MSR_LBR_NHM_TO   + i, lbr->to  [i]);
    }
}

/* Dump the LBR registers as stored in <lbr>. */
void dump_lbr(struct lbr_t *lbr) {
    int i;
    printd(true, "MSR_IA32_DEBUGCTLMSR: 0x%llx\n", lbr->debug);
    printd(true, "MSR_LBR_SELECT:       0x%llx\n", lbr->select);
    printd(true, "MSR_LBR_TOS:          %lld\n", lbr->tos);
    for (i = 0; i < LBR_ENTRIES; i++) {
      printd(true, "MSR_LBR_NHM_FROM[%2d]: 0x%llx\n", i, lbr->from[i]);
      printd(true, "MSR_LBR_NHM_TO  [%2d]: 0x%llx\n", i, lbr->to[i]);
    }
}

/* Enable the LBR feature for the current CPU. *info may be NULL (it is required
 * by on_each_cpu()).
 */
void enable_lbr(void *info) {

    get_cpu();

    printd(true, "Enabling LBR\n");

    /* Apply the filter (what kind of branches do we want to track?) */
    wrmsrl(MSR_LBR_SELECT, LBR_SELECT);
    
    /* Flush the LBR and enable it */
    flush_lbr(true);

    put_cpu();
}

/* Disable the LBR feature for the current CPU
 */

void disable_lbr(void *info) {

   get_cpu();

   printd(true, "Disabling LBR\n");

   flush_lbr(false);

   put_cpu();
   
}
