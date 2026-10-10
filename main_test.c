#include <stdio.h>
int main(int argc, char const *argv[])
{
    if(argc >= 2){
        for(int i=0;i<argc;i++)
        {
            printf("%dparam is %s\n",i,argv[i]);
        }
    }
    else
    {
        printf("paramOnly:%s\n",argv[0]);
    }
    return 0;
}
