#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    printf("parents\n");

    int child_proesess_status;
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid==0)
    {
        char* args[]={"/usr/bin/ping","-c","10","www.baidu.com",NULL};
        char* evns[]={NULL};
        printf("ping number 10\n");
        execve(args[0],args,evns);
    }
    else
    {
        printf("parents(%d) is waitting child(%d)\n",getpid(),pid);
        waitpid(pid,&child_proesess_status,0);
    }
    printf("waitted\n");
}

