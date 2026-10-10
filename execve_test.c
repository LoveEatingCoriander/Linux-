#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    char *argv[] = {"hello", "world", NULL};
    char *envp[] = {NULL};
    execve("/home/hhh/process_demo/main_test", argv, envp);
    return 0;
}

