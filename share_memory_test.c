#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    char path[100]={0};
    sprintf(path,"/letter%d",getpid());
    int fd=shm_open(path,O_RDWR|O_CREAT,0644);
    if(fd<0)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd,1024);
    while(1)
    {

    }
    return 0;
}
