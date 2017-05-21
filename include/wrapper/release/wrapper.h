#ifndef __WRAPER_H__
#define __WRAPER_H__

#include "lbr.h"

void increment_nhits(void);

struct lbr_entry{
  unsigned long long tos;
  unsigned long long from;
};

#define MAX_OFFSET 8

#endif



