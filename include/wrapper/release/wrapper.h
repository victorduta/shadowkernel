#ifndef __WRAPER_H__
#define __WRAPER_H__

#include "lbr.h"

struct lbr_entry{
  unsigned long long tos;
  unsigned long long from;
};

#define MAX_OFFSET 8


#if !defined(SKIP_INSTRUMENTATION) && !defined(SKIP_SCRIPT_INSTRUMENTATION)
DECLARE_PER_CPU(uint64_t, n_hits);
DECLARE_PER_CPU(uint64_t, n_misses);
#endif
	

#endif



