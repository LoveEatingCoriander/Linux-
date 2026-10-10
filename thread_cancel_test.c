#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

void *task(void *argv)
{
    printf("task started\n");
    pthread_testcancel();
    sleep(1);
    printf("task end\n");
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t tid;
    pthread_create(&tid,NULL,&task,NULL);
    //pthread_cancel(tid);
    pthread_detach(tid);
    sleep(3);
    return 0;
}
