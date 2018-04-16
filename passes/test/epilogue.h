#ifndef __EPILOGUE_H__
#define __EPILOGUE_H__
#include <stdio.h>
void __attribute__((weak)) lbr_epilogue(void* addr, unsigned long long* sig);

void lbr_epilogue(void *addr, unsigned long long* sig)
{

   printf("Lbr Epilogue %d\n", *sig);
}

unsigned long long getXXSignature();
void setXXSignature(unsigned long long value);

#endif
