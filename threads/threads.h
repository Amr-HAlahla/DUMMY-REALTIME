#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
pthread_barrier_t barrier;

/*
how to get process id of a thread ?
    ==> printf("Thread id: %d\n", syscall(SYS_gettid));
*/