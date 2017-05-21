#ifndef __EPILOGUE_H__
#define __EPILOGUE_H__

#include "wrapper.h"

void __attribute__((weak)) lbr_epilogue(void* addr);

#ifdef PAD_AT_IR
void __attribute__((weak)) pad_2nops();
void __attribute__((weak)) pad_5nops();
#endif

void lbr_epilogue(void *addr)
{   
    check_wrapper(addr);
}

#ifdef PAD_AT_IR
void pad_2nops()
{
__asm__ __volatile__ (
		"nop\n\t"
		"nop\n\t"
		: 
		: 
	);
}

void pad_5nops()
{
    __asm__ __volatile__ (
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
                "nop\n\t"
		: 
		: 
	);
}
#endif

#endif
