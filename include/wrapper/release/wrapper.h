#ifndef __WRAPER_H__
#define __WRAPER_H__

#include "lbr.h"

typedef struct address_entry {
   unsigned long long tos;
   unsigned long long from;
   unsigned long long to;
   unsigned long long address;
   unsigned long long nhits;
} addr_entry;

#define NUM_ENTRIES 50000
#define NUM_UNITS 8

#ifdef __KERNEL__
struct lbr_entry{
  unsigned long long tos;
  unsigned long long from;
  unsigned long long to;
};

#define MAX_OFFSET 8


#if !defined(SKIP_INSTRUMENTATION) && !defined(SKIP_SCRIPT_INSTRUMENTATION)
DECLARE_PER_CPU(uint64_t, n_hits);
DECLARE_PER_CPU(uint64_t, n_misses);
#endif


typedef void (*add_lbr_entry_func_t)(struct address_entry *entry);
extern add_lbr_entry_func_t add_lbr_entry;

//#if !defined(SKIP_INSTRUMENTATION) && !defined(SKIP_SCRIPT_INSTRUMENTATION)
//DECLARE_PER_CPU(addr_entry[NUM_ENTRIES] , lbr_percpu_entries);
//#endif
	
#endif // __KERNEL__

#endif // __WRAPER_H__



