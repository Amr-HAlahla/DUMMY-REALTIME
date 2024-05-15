#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define SEM_KEY 1234
#define SEM_CLIENT1 0
#define SEM_CLIENT2 1

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void wait_semaphore(int sem_id, int sem_num)
{
    struct sembuf semaphore;
    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1; /* -1 means wait */
    semaphore.sem_flg = 0;
    semop(sem_id, &semaphore, 1); /* 1 is the number of semaphore operations to perform */
}

void signal_semaphore(int sem_id, int sem_num)
{
    struct sembuf semaphore;
    semaphore.sem_num = sem_num;
    semaphore.sem_op = 1; /* 1 means signal */
    semaphore.sem_flg = 0;
    semop(sem_id, &semaphore, 1);
}

#endif