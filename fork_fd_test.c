#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    int fd = open("io.txt", O_APPEND | O_CREAT | O_WRONLY, 0644);

    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    char buffer[32];

    if (pid < 0)
    {
        perror("fork");
        close(fd);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        sleep(1);
        strcpy(buffer, "child write\n");
    }
    else
    {
        strcpy(buffer, "parent write\n");
    }

    ssize_t bytes_write = write(fd, buffer, strlen(buffer));
    if (bytes_write == -1)
    {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    if (pid == 0)
    {
        printf("child over\n");
    }
    else
    {
        printf("parent over\n");
        wait(NULL);  
    }

    return 0;
}