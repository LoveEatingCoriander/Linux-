#include <stdio.h>
#include <stdlib.h>
int main()
{
    int sysR = system("ping -c 100 www.baidu.com");
    if(sysR != 0)
    {
        perror("system");
        exit(EXIT_FAILURE);
    }
    return 0;
}