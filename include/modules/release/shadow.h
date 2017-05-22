#ifndef __SHADOW_H__
#define __SHADOW_H__

#ifdef __KERNEL__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h>
#endif

#define SHADOW_IOC_MAGIC 'l'
#define SHADOW_IOC_TEST                   _IO(SHADOW_IOC_MAGIC,  1)
#define SHADOW_MINOR 133

#endif /* __SHADOW_H__ */
