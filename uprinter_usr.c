#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include"commands.h" /* contains IOCTL commands */

#define IMAGE_PEL 82 /* Image Size 82x82 pixels */

/* 
sendCommand():void
common function for all ioctl commands
*/
void sendCommand(unsigned int cmdType, unsigned long duration)
{
    int fd,ret_val;
    fd=open("/dev/uprinter_dev",O_WRONLY,S_IWUSR);
    if(fd<0)
    printf("USB Printer not found\n");
    else
    {
        ret_val=ioctl(fd,cmdType,duration);
        close(fd);
        if(ret_val<0)
        printf("Printing Failed\n");
    }
}

/*
writeDot():void
Triggered when data to print is present
Moves write-pen down by controlling servo motion
Turns on an LED to indicate data-write
*/
void writeDot()
{
    sendCommand(MOVE_PEN_DOWN,1);
    sendCommand(MOVE_PEN_HOLD,1);
}

int main(int argc,char *argv[])
{
    int index,xIndex,yIndex;
    char buffer[20],value;
    char fileData[IMAGE_PEL*IMAGE_PEL]; /* Image represented as 1-D array */
    int option;
    FILE *fp;
    
    while(1)
    {
        printf("---------------[USB Dot-Matrix Printing Interface]---------------\n");
        printf("1: Load input image file\n");
        printf("2: Print 82x82 image\n");
        scanf("%d",&option);
        
        if(option==1)
        {
            printf("Enter name of picture file\n");
            scanf("%s",buffer);
            fp=fopen(buffer,"r");
            index=0;
    	    while((value=getc(fp))!=EOF)
            {
        	if(value=='1' || value == '0')
        	{
                    fileData[index++]=value;
        	}
        	if(index==IMAGE_PEL*IMAGE_PEL) /* exits on array full */
        	    break;
    	    }
            fclose(fp);
        }
        else if(option==2)
        {
            /* Initial housekeeping resets both the stepper motors to initial position of 0,0 */
            printf("Housekeeping...");
            /* 820 steps for both motors */
            for(index=0;index<16;++index)
            	sendCommand(MOVE_PAGE_BCK,50);
            sendCommand(MOVE_PAGE_BCK,20);
            for(index=0;index<16;++index)
            	sendCommand(MOVE_CHAR_BCK,50);
            sendCommand(MOVE_CHAR_BCK,20);
            printf("done\n");
            printf("Printing Image...");
            /* Iterate over image and print according to its binary bits */
            for(yIndex=0;yIndex<IMAGE_PEL;++yIndex)
            {
            	for(xIndex=0;xIndex<IMAGE_PEL;++xIndex)
                {
                    if(fileData[yIndex*IMAGE_PEL+xIndex]=='1')
                    {  
                        writeDot();
                    }
                    sendCommand(MOVE_CHAR_FWD,10); /* Move to print next character */
                }
                /* one line finishes here */
                /* character printing motor is moved back 820 steps */
                for(index=0;index<16;++index)
            	    sendCommand(MOVE_CHAR_BCK,50);
            	sendCommand(MOVE_CHAR_BCK,20);
            	/* page is forwarded by 1 line */
                sendCommand(MOVE_PAGE_FWD,10);
            }
            printf("done\n");
        }
        else
            break;
    }
}
