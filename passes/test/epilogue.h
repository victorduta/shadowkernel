#ifndef __EPILOGUE_H__
#define __EPILOGUE_H__

void __attribute__((weak)) lbr_epilogue(void* addr);

void lbr_epilogue(void *addr)
{

   printf("Lbr Epilogue\n");
}

#endif
