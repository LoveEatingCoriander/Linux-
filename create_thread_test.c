#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_LEN 1024

char *buf;

void *tread(void *argv)
{
    int i = 0;
    while (1)
    {
        char c = fgetc(stdin);
        if (c && c != '\n')
        {
            buf[i++] = c;
        }
        if (i >= BUF_LEN)
        {
            i = 0;
        }
    }
}

void *twrite(void *argv)
{
    int i=0;
    while (1)
    {
        if(buf[i])
        {
            printf("message is ");
            fputc(buf[i],stdout);
            fputc('\n',stdout);
            buf[i++]=0;
        }
        if(i>=BUF_LEN)
        {
            i=0;
        }
        if(buf[i]==0)
        {
            sleep(1);
        }
    }
    
}

int main(int argc, char const *argv[])
{
    pthread_t tid_input;
    pthread_t tid_output;
    buf=(char*)malloc(sizeof(char)*BUF_LEN);
    //read thread
    pthread_create(&tid_input, NULL,tread,NULL); 
    //write thread
    pthread_create(&tid_output,NULL,twrite,NULL);
    //wait these thread over
    pthread_join(tid_input,NULL);
    pthread_join(tid_output,NULL);
    free(buf);
    return 0;
}
