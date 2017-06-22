#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "shadow.h"

#include "wrapper.h"

#define INIT_ENTRIES "init_entries"
#define GET_ENTRIES "get_entries"
#define GET_CYCLES "get_cycles"

struct address_entry addr_entries[NUM_ENTRIES];

static void print_entries()
{
   int i;
   for(i = 0; i < NUM_ENTRIES; i++)
   {
       if(addr_entries[i].to == 0)
       {
           break;
       }
       printf("%llx %lld\n", addr_entries[i].to, addr_entries[i].nhits);
   }
}

static void interpret_cmd(int fd, char *cmd)
{
   int ret;
   int n_offset;
   if(!strcmp(cmd, INIT_ENTRIES))
   {
        printf("Initializing array of recorded functions...\n");
        ioctl(fd, SHADOW_IOC_INIT_ENTRIES);
        return;
   }
   
   if(!strcmp(cmd, GET_ENTRIES))
   {
        memset(addr_entries, 0, NUM_ENTRIES*sizeof(struct address_entry));
        printf("Getting recorded entries...\n");
        n_offset = 0;
        while((ret = ioctl(fd, SHADOW_IOC_GET_ENTRIES, &addr_entries[n_offset])) > 0)
        {
            n_offset += ret;
        }
        if (!ret)
        {
           print_entries();
        }
        return;
   }
   
   if(!strcmp(cmd, GET_CYCLES))
   {
      ioctl(fd, SHADOW_IOC_GET_CYCLES);
      return;
   }

}
int main(int argc,  char *argv[])
{
   int shadow_fd;
   int i;
   
   shadow_fd = open("/dev/shadow_lbr", O_RDWR);
 
   if(shadow_fd < 0)
   {
       printf("Error opening shadow lbr interface\n");
       return 1;
   }
   for(i = 1; i < argc; i++)
   {
      interpret_cmd(shadow_fd, argv[i]);
   } 
   //ioctl(shadow_fd, SHADOW_IOC_TEST);
   //ioctl(shadow_fd, SHADOW_IOC_GET_CYCLES);
   close(shadow_fd);
}
