#include<stdio.h>

#define IMAGE_PEL 82

int main(int argc,char *argv[])
{
    int fd,index,xIndex,yIndex;
    int ret_val;
    char buffer[20],value;
    char fileData[IMAGE_PEL*IMAGE_PEL];
    FILE *fp;
    printf("---------------[USB Dot-Matrix Picture Debugging]---------------\n");
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
        if(index==IMAGE_PEL*IMAGE_PEL)
            break;
    }
    for(index=0;index<IMAGE_PEL*IMAGE_PEL;++index)
    {
        if(fileData[index]=='1')
        printf("*");
        else
        printf(" ");
        if(index%82==81)
        printf("\n");
    }
    fclose(fp);
}
