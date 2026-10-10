#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int signal_num)
{
    printf("\nreceive signal i:%d\n",signal_num);
    exit(signal_num);
}

int main(int argc, char const *argv[])
{
    if(signal(SIGINT,signal_handler)==SIG_ERR)
    {
        perror("signal");
        return 1;
    }
    while(1)
    {
        sleep(1);
        printf("hello\n");
    }
    return 0;
}
