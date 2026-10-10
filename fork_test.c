#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    printf("older process id:%d\n",getpid());

    pid_t p1;
    p1 = fork();
    printf("p1 id:%d\n",p1);
    return 0;
}