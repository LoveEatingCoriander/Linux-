#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#define THREAD_COUNT 20000
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *add_thread(void *argv)
{
    int *p=(int *)argv;
    pthread_mutex_lock(&mutex);
    (*p)++;
    pthread_mutex_unlock(&mutex);
    return p;
}

int main(int argc, char const *argv[])
{
    pthread_t pid[THREAD_COUNT];
    int num=0;
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&pid[i],NULL,add_thread,&num);
    }
    for(int i=0;i<THREAD_COUNT;i++)
    {
        pthread_join(pid[i],NULL);
    }
    printf("%d\n",num);
    return 0;
}
