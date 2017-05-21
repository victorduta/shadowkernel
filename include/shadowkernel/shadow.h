#ifndef __SHADOW_H__
#define __SHADOW_H__

#ifdef __KERNEL__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h>
#endif

#define SHADOW_IOC_MAGIC 'l'
#define SHADOW_IOC_TEST                   _IO(SHADOW_IOC_MAGIC,  1)
#define SHADOW_IOC_START_RECORDING        _IO(SHADOW_IOC_MAGIC,  2)
#define SHADOW_IOC_GET_RECORDS            _IO(SHADOW_IOC_MAGIC,  3)
#define SHADOW_IOC_INSTALL_DEFAULT_HOOK   _IO(SHADOW_IOC_MAGIC,  4)
#define SHADOW_MINOR 133

#include "lbr-state.h"

struct shadow_record_t
{
    int cpu; // running cpu
    uint64_t addr; // return address
    struct lbr_t lbr_state;
};

#ifndef __KERNEL__
static inline void dump_shadow_record(struct shadow_record_t *record)
{
   printf("CPU: 0x%d\n", record->cpu);
   printf("RETURN_ADDR: 0x%llx\n", record->addr);
   dump_lbr_record(&record->lbr_state);
}
#endif

#endif /* __SHADOW_H__ */
