#ifndef __EPILOGUE_H__
#define __EPILOGUE_H__


#if !defined(SKIP_SCRIPT_INSTRUMENTATION) && !defined(SKIP_INSTRUMENTATION)
#include <linux/kernel.h>
#include <linux/sched.h>
#endif

#ifndef SKIP_INSTRUMENTATION
#include <linux/preempt.h>
#endif

#include "wrapper.h"

#define BENCHMARK_RDMSR


void __attribute__((weak)) lbr_epilogue(void* addr);


void lbr_epilogue(void *frame)
{  
   struct lbr_entry lbr;
#ifndef SKIP_INSTRUMENTATION
   unsigned long long *address_slot = (unsigned long long*)((char *)frame+8);
#ifdef BENCHMARK_RDMSR
   preempt_disable(); 
   rdmsrl(MSR_LBR_TOS,  lbr.tos);
   rdmsrl(MSR_LBR_NHM_FROM + lbr.tos, lbr.from);
   preempt_enable();
#endif
#ifdef BENCHMARK_RA_REWRITE
   lbr.from = LBR_FROM(lbr.from);
   if( (*address_slot < lbr.from) || (*address_slot > (lbr.from+MAX_OFFSET)))
   {
#ifndef SKIP_SCRIPT_INSTRUMENTATION
        get_cpu_var(n_misses)++;
        put_cpu_var(n_misses);
#endif
        return;
   }
   else
   {
#ifndef SKIP_SCRIPT_INSTRUMENTATION
        get_cpu_var(n_hits)++;
        put_cpu_var(n_hits);
#endif
       *address_slot = lbr.from+MAX_OFFSET;
   }
#endif
#endif
}


#endif
