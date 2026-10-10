#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char mq_name[]="/father_son_mq";
    struct mq_attr attr={
        .mq_flags=0,
        .mq_maxmsg=10,
        .mq_msgsize=100,
        .mq_curmsgs=0,
    };
    mqd_t mqdes= mq_open(mq_name,O_RDWR|O_CREAT,0664,&attr);
    if(mqdes==(mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    pid_t cpid =fork();
    if(cpid<0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(cpid==0)
    {
        char buf[100];
        struct timespec time_info;
        for (int i = 0; i < 10; i++)
        {
            memset(buf,0,sizeof(buf));
            clock_gettime(CLOCK_REALTIME,&time_info);
            time_info.tv_sec+=5;
            if(mq_timedreceive(mqdes,buf,100,0,&time_info)==-1)
            {
                perror("mq_rec");
            }
            printf("child receive message :%s\n",buf);
        }
        
    }
    else
    {
        char buf[100];
        struct timespec time_info;
        for(int i=0;i<10;i++)
        {
            memset(buf,0,sizeof(buf));
            sprintf(buf,"parent send message:%d\n",(int)i);
            clock_gettime(CLOCK_REALTIME,&time_info);
            time_info.tv_sec+=5;
            if(mq_timedsend(mqdes,buf,100,0,&time_info)==-1)
            {
                perror("mq_send");
            }
            printf("sleep 1 sec\n");
            sleep(1);
        }
        
    }
    mq_close(mqdes);
    if(cpid>0)
    {
        mq_unlink(mq_name);
    }
    return 0;
}


