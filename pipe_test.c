#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char const *argv[])
{
    pid_t cpid;
    int pipefd[2];
    if(argc!=2)
    {
        fprintf(stderr,"%s:please write param\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    if(pipe(pipefd)==-1)
    {
        perror("pipe failed\n");
        exit(EXIT_FAILURE);
    }
    cpid=fork();
    if(cpid==-1)
    {
        perror("fork error\n");
        exit(EXIT_FAILURE);
    }
    else if(cpid==0)
    {
        close(pipefd[1]);
        char buf[100]={0};
        char *str="child start read\n";
        write(STDOUT_FILENO,str,strlen(str));
        while(read(pipefd[0],&buf,1)>0)
        {
            write(STDOUT_FILENO,&buf,1);
        }
        write(STDOUT_FILENO,"\n",1);
        close(pipefd[0]);
    }
    else
    {
        close(pipefd[0]);
        printf("parent(%d) send message : %s\n",getpid(),argv[1]);
        write(pipefd[1],argv[1],strlen(argv[1]));
        close(pipefd[1]);
        waitpid(cpid,NULL,0);//wait child over
        exit(EXIT_SUCCESS);
    }
    return 0;
}
