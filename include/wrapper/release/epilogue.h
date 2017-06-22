#ifndef __EPILOGUE_H__
#define __EPILOGUE_H__


#if !defined(SKIP_SCRIPT_INSTRUMENTATION) && !defined(SKIP_INSTRUMENTATION)
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#endif

#ifndef SKIP_INSTRUMENTATION
#include <linux/preempt.h>
#endif

#include "wrapper.h"



void __attribute__((weak)) lbr_epilogue(void* addr);

#ifndef INCLUDE_MEASURE_INSTRUMENTATION
void lbr_epilogue(void *frame)
{  
   struct lbr_entry lbr;
   unsigned long flags;
#ifndef SKIP_INSTRUMENTATION
   unsigned long long *address_slot = (unsigned long long*)((char *)frame+8);
#ifdef INCLUDE_RDMSR
   preempt_disable(); 
   rdmsrl(MSR_LBR_TOS,  lbr.tos);
   rdmsrl(MSR_LBR_NHM_FROM + lbr.tos, lbr.from);
   preempt_enable();
#endif
#ifdef INCLUDE_RA_REWRITE
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
#else
void lbr_epilogue(void *frame)
{ 

#if !defined(SKIP_SCRIPT_INSTRUMENTATION) && !defined(SKIP_INSTRUMENTATION)
   struct address_entry entry;
   unsigned long long *address_slot = (unsigned long long*)((char *)frame+8);
   
   preempt_disable(); 
   rdmsrl(MSR_LBR_TOS,  entry.tos);
   rdmsrl(MSR_LBR_NHM_FROM + entry.tos,  entry.from);
   rdmsrl(MSR_LBR_NHM_TO + entry.tos, entry.to);
   preempt_enable();

   entry.from = LBR_FROM(entry.from);
   entry.address = *address_slot;
   add_lbr_entry(&entry);
#endif
}
#endif

#endif
