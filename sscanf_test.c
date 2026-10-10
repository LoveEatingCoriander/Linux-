#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(int argc, char const *argv[])
{
    int fd=open("./io.txt",O_RDONLY, 0644);
    if(fd==-1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buf[32]={0};
    int n=0;
    while((n = read(fd, buf, 32)) > 0)
    {
        write(STDOUT_FILENO,buf,n);
        //printf("\n");
    }
    
    return 0;
}
