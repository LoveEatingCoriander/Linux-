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
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_curmsgs = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = 100,
    };
    char *mq_name = "/mq_producer_consumer";
    mqd_t mqdes = mq_open(mq_name, O_RDWR | O_CREAT, 0664, &attr);
    if (mqdes == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    char buf[100];
    struct timespec time_info;
    while(1)
    {
        memset(buf,0,sizeof(buf));
        clock_gettime(CLOCK_REALTIME,&time_info);
        time_info.tv_sec+=15;
        if(mq_timedreceive(mqdes,buf,sizeof(buf),0,&time_info)==-1)
        {
            perror("mq_timedreceive");
        }
        else
        {
            if(buf[0]==EOF)
            {
                printf("receive message:over\n");
                break;
            }
            else
            {
                printf("receive message:%s\n",buf);
            }
        }
    }
    mq_close(mqdes);
    mq_unlink(mq_name);
    return 0;
}
