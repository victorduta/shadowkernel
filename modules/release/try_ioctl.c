#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "shadow.h"


int main()
{
   int shadow_fd;

   if(shadow_fd < 0)
   {
       printf("Error opening shadow lbr interface\n");
       return 1;
   }

   ioctl(shadow_fd, SHADOW_IOC_TEST);

   close(shadow_fd);
}
