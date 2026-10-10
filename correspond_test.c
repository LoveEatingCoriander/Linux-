#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int num;
int main(int argc, char const *argv[])
{
    int ret=fork();
    if(ret<0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(ret==0)
    {
        num=1;
        printf("child num adress and value: %d,%d\n",&num,num);
    }
    else
    {
        sleep(1);
        printf("parent num adress and value:%d,%d\n",&num,num);
    }
    return 0;
}
