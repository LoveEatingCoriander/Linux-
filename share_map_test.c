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
    char *path="/share_map";
    int fd=shm_open(path,O_RDWR| O_CREAT,0644);
    if(fd<0)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd,1024);

    char *share=mmap(NULL,1024,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(share==MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    close(fd);

    pid_t cpid=fork();
    if(cpid<0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(cpid==0)
    {
        strcpy(share,"you are good people");
        write(STDOUT_FILENO,"write over\n",11);
    }
    else
    {
        waitpid(cpid,NULL,0);
        printf("parents(%d) get message :%s\n",getpid(),share);
    }

    munmap(share,1024);

    shm_unlink(path);
    return 0;
}
