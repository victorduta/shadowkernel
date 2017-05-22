
#include "wrapper.h"

DEFINE_PER_CPU(uint64_t, n_hits) = 0;

EXPORT_PER_CPU_SYMBOL(n_hits);

DEFINE_PER_CPU(uint64_t, n_misses) = 0;

EXPORT_PER_CPU_SYMBOL(n_misses);




