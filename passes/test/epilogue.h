#ifndef __EPILOGUE_H__
#define __EPILOGUE_H__

void __attribute__((weak)) lbr_epilogue(void* addr);
#ifdef PAD_NOPS
void __attribute__((weak)) pad_2nops();
void __attribute__((weak)) pad_5nops();
#endif
int test_global = 3;
void lbr_epilogue(void *addr)
{
    test_global++;
    if(test_global == 4)
    {
       printf("Mother\n");
       return;
    }
    test_global++;
    return;
}

#ifdef PAD_NOPS
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
