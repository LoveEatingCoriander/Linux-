#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
static pthread_rwlock_t rw_lock;

void *write_thread(void *argv)
{
    int *p=(int *)argv;
    pthread_rwlock_wrlock(&rw_lock);
    sleep(1);
    (*p)++;
    pthread_rwlock_unlock(&rw_lock);
    return NULL;
}

void *read_thread(void *argv)
{
    int *p=(int *)argv;
    pthread_rwlock_rdlock(&rw_lock);
    printf("%d\n",(*p));
    pthread_rwlock_unlock(&rw_lock);
}

int main(int argc, char const *argv[])
{
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr,PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    pthread_rwlock_init(&rw_lock,&attr);
    pthread_t w_pid[2];
    pthread_t r_pid[3];
    int num=0;
    pthread_create(&w_pid[0],NULL,write_thread,&num);
    pthread_create(&w_pid[1],NULL,write_thread,&num);
    
    pthread_create(&r_pid[0],NULL,read_thread,&num);
    pthread_create(&r_pid[1],NULL,read_thread,&num);
    pthread_create(&r_pid[2],NULL,read_thread,&num);

    pthread_join(w_pid[0],NULL);
    pthread_join(w_pid[1],NULL);
    pthread_join(r_pid[0],NULL);
    pthread_join(r_pid[1],NULL);
    pthread_join(r_pid[2],NULL);
    return 0;
}
