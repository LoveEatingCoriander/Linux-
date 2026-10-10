#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

typedef struct 
{
    char *p;
    int len;
}Result;

void *red(void *param)
{
    Result *res=(Result *)malloc(sizeof(Result));
    char code=*((char *)param);

    char buf[100];
    while(1)
    {
        fgets(buf,100,stdin);
        if(buf[0]==code)
        {
            printf("red exit\n");
            char *redans=strdup("red exit\n");
            res->p=redans;
            res->len=strlen(redans);
            pthread_exit((void*)res);
        }
        else
        {
            printf("red is waitting\n");
        }
    }
    
}

void *white(void *param)
{
    Result *res=(Result *)malloc(sizeof(Result));
    char code=*((char *)param);

    char buf[100];
    while(1)
    {
        fgets(buf,100,stdin);
        if(buf[0]==code)
        {
            printf("white exit\n");
            char *whiteans=strdup("white exit\n");
            res->p=whiteans;
            res->len=strlen(whiteans);
            pthread_exit((void*)res);
        }
        else
        {
            printf("white is waitting\n");
        }
    }
}

int main(int argc, char const *argv[])
{
    pthread_t tid_red;
    pthread_t tid_white;
    char red_code='r';
    char white_code='w';
    pthread_create(&tid_red,NULL,red,(void *)&red_code);
    pthread_create(&tid_white,NULL,white,(void *)&white_code);

    Result *red_res=NULL;
    Result *white_res=NULL;
    pthread_join(tid_red,(void **)&red_res);
    printf("red result is %s\n",red_res->p);
    pthread_join(tid_white,(void **)&white_res);
    printf("white result is %s\n",white_res->p);
    return 0;
}
