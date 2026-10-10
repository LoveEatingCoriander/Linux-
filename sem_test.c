#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int num=0;
    sem_t sem;
void* add(void*)
{
    sem_wait(&sem);
    num++;
    sem_post(&sem);
}

int main(int argc, char const *argv[])
{

    int sem_ret=sem_init(&sem,0,1);
    if(sem_ret!=0)
    {
        perror("sem");
        exit(EXIT_FAILURE);
    }
    pthread_t pthid[10000];
    for(int i=0;i<10000;i++)
    {
        pthread_create(&pthid[i],NULL,add,NULL);
    }
    for(int i=0;i<10000;i++)
    {
        pthread_join(pthid[i],NULL);
    }
    printf("%d\n",num);
    sem_destroy(&sem);
    
    return 0;
}
