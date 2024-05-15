#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

#define SHM_SIZE 1024

// Semaphore union for semctl
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

#endif