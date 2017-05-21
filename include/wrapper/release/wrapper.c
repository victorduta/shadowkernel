
#include "wrapper.h"

DEFINE_PER_CPU(int, n_hits) = 0;

EXPORT_PER_CPU_SYMBOL_GPL(n_hits);

void increment_nhits()
{
   int *p_hits = &get_cpu_var(n_hits);
    (*p_hits)++;
   put_cpu_var(n_hits);
}

EXPORT_SYMBOL(increment_nhits);


