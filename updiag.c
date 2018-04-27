#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include"commands.h"

int main(int argc,char *argv[])
{
    int fd;
    int ret_val;
    int duration;
    printf("---------------[USB Dot-Matrix Debug Interface]---------------\n");
        if(argc==2)
        {
           fd=open("/dev/uprinter_dev",O_WRONLY,S_IWUSR);
            if(fd<0)
            printf("USB Printer not found\n");
            else
            {
            printf("\nEnter duration ");
            scanf("%d",&duration);
                switch(argv[1][0])
                {
                    case '1':
                    ret_val=ioctl(fd,MOVE_PAGE_FWD,duration);
                    break;
                    case '2':
                    ret_val=ioctl(fd,MOVE_PAGE_BCK,duration);
                    break;
                    case '3':
                    ret_val=ioctl(fd,MOVE_CHAR_FWD,duration);
                    break;
                    case '4':
                    ret_val=ioctl(fd,MOVE_CHAR_BCK,duration);
                    break;
                    case '5':
                    ret_val=ioctl(fd,MOVE_PEN_DOWN,1);
                    break;
                    case '6':
                    ret_val=ioctl(fd,MOVE_PEN_HOLD,1);
                    break;
                }
                
                close(fd);
                if(ret_val<0)
                printf("Printing Failed\n");
            }
       }
}
