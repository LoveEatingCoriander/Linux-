#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    FILE* fd = fopen("./file.txt", "a+");
    if(fd==NULL)
    {
        printf("error:%d\n",errno);
        perror("Open");
    }
    char buf[32]="123456789nihao";
    fputs(buf,fd);
    memset(buf,0,sizeof(buf));
    fd = fopen("./file.txt", "r+");
    fgets(buf,sizeof(buf),fd);
    printf("%s",buf);

    return 0;
}
