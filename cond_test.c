#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count=0;

static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

//write data into buffer
void* pproducer(void* argv)
{
    int item=1;
    pthread_mutex_lock(&mutex);
    while(1)
    {
        if(count==BUFFER_SIZE)
        {
            pthread_cond_wait(&cond,&mutex);
        }
        buffer[count++]=item++;
        printf("producer write %d\n",buffer[count-1]);
        pthread_cond_signal(&cond);
    }
}

//read data from buffer
void* pconsumer(void* argv)
{
    pthread_mutex_lock(&mutex);
    while (1)
    {
        if(count==0)
        {
            pthread_cond_wait(&cond,&mutex);
        }
        printf("consumer receive %d\n",buffer[--count]);
        pthread_cond_signal(&cond);
    }    
}

int main(int argc, char const *argv[])
{
    pthread_t producer,consumer;
    pthread_create(&producer,NULL,pproducer,NULL);
    pthread_create(&consumer,NULL,pconsumer,NULL);

    pthread_join(producer,NULL);
    pthread_join(consumer,NULL);

    return 0;
}
