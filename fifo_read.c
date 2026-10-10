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
    
    fd=open(path,O_RDONLY);
    if(fd==-1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buf[100];
    ssize_t read_num;
    while((read_num=read(fd,buf,100))>0)
    {
        write(STDOUT_FILENO,buf,read_num);
    }
    if(read_num<0)
    {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("fifo_write over\n");
    close(fd);
    exit(EXIT_SUCCESS);
    return 0;
}
