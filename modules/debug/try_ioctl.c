#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "shadow.h"

#define RECORDS_NEEDED 1000

#define MAX_OFFSET 10

static void analyse_record(struct shadow_record_t *record)
{
    int i;
    uint64_t offset;
    printf("BEGIN RECORD ANALYSIS\n");
    for(i=0; i < LBR_ENTRIES; i++)
    {
       if ( (offset = (int)(record->addr - record->lbr_state.from[i])) <= MAX_OFFSET)
       {
          printf("Entry num:%d\n", i);
          printf("Entry offset:%lx\n", offset); 
          printf("Offset from TOS:%d\n", (((record->lbr_state.tos - i) + 16) % 16));
       } 
    }
    printf("END RECORD ANALYSIS\n");
}

int main()
{
    int shadow_fd, n_read = 0, n_offset = 0;
    int num_records = RECORDS_NEEDED;
    int i;
    struct shadow_record_t records[RECORDS_NEEDED];

    shadow_fd = open("/dev/shadow_lbr", O_RDWR);

    if(shadow_fd < 0)
    {
       printf("Error opening shadow lbr interface\n");
       return 1;
    }

    ioctl(shadow_fd, SHADOW_IOC_TEST);
/*

    ioctl(shadow_fd, SHADOW_IOC_START_RECORDING, &num_records);

    while((n_read = ioctl(shadow_fd, SHADOW_IOC_GET_RECORDS, &records[n_offset])) >= 0)
    {
          n_offset += n_read;
    }

    for (i = 0; i < num_records; i++)
    {
        analyse_record(&records[i]);
        dump_shadow_record(&records[i]);
    }
*/
    close(shadow_fd);

    
}
