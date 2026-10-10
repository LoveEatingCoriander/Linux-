#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{
    int fd;
    char* path="/tmp/myfifo";
    if(mkfifo(path,0664)!=0 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    fd=open(path,O_WRONLY);
    if(fd==-1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buf[100];
    ssize_t read_num;
    while((read_num=read(STDIN_FILENO,buf,100))>0)
    {
        write(fd,buf,read_num);
    }
    if(read_num<0)
    {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("fifo_write over\n");
    close(fd);
    //release pipo
    if(unlink(path)==-1)
    {
        perror("unlink");
    }
    exit(EXIT_SUCCESS);

    return 0;
}