#ifndef __EPILOGUE_H__
#define __EPILOGUE_H__


#include "wrapper.h"


#ifndef SKIP_INSTRUMENTATION
#ifndef SKIP_SCRIPT_INSTRUMENTATION
#include <linux/kernel.h>
#include <linux/sched.h>
#endif
#include <linux/preempt.h>
#endif


void __attribute__((weak)) lbr_epilogue(void* addr);


void lbr_epilogue(void *frame)
{  
   struct lbr_entry lbr;
#ifndef SKIP_INSTRUMENTATION
   unsigned long long *address_slot = (unsigned long long*)((char *)frame+8);
   preempt_disable(); 
   rdmsrl(MSR_LBR_TOS,  lbr.tos);
   rdmsrl(MSR_LBR_NHM_FROM + lbr.tos, lbr.from);
   preempt_enable();

   lbr.from += MAX_OFFSET;
   if( *address_slot > lbr.from)
   {
        return;
   }
   else
   {
       *address_slot = lbr.from;
   }
#endif
}


#endif
