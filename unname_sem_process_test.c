#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
int main(int argc, char const *argv[])
{
    char value_path[] = "/value_path";
    char sem_path[] = "/sem_path";

    int value_fd = shm_open(value_path, O_CREAT | O_RDWR, 0666);
    int sem_fd = shm_open(sem_path, O_CREAT | O_RDWR, 0666);

    ftruncate(value_fd, sizeof(int));
    ftruncate(sem_fd, sizeof(sem_t));

    int *value = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, value_fd, 0);
    sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, sem_fd, 0);

    close(value_fd);
    close(sem_fd);

    *value = 0;
    sem_init(sem, 1, 1);

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        sem_wait(sem);
        printf("child receive %d\n", *value);
        sem_post(sem);
    }
    else
    {
        sem_wait(sem);
        sleep(1);
        (*value)++;
        sem_post(sem);
        printf("parent write %d\n", *value);
        waitpid(pid, NULL, 0);
        sem_destroy(sem);
        shm_unlink(value_path);
        shm_unlink(sem_path);
    }

    if (munmap(value, sizeof(int)) == -1)
    {
        perror("munmap value");
        exit(EXIT_FAILURE);
    }
    if (munmap(sem, sizeof(int)) == -1)
    {
        perror("munmap sem");
        exit(EXIT_FAILURE);
    }
 
    return 0;
}
