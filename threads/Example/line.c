#include "includes.h"
#include "functions.c"

#define EMPLOYEE 10
pthread_t employees[EMPLOYEE];
pthread_barrier_t barrier;
pthread_barrier_t order_barrier;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int steps_count = 1;
int state[EMPLOYEE] = {0};

void *routine(void *arg)
{
    int id = *(int *)arg + 1;
    pthread_barrier_wait(&barrier);
    if (id <= 5)
    {
        pthread_mutex_lock(&mutex);
        while (steps_count != id)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Step %d Done by Thread %d\n", steps_count, id);
        steps_count++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
        pthread_barrier_wait(&order_barrier);
    }
    else
    {
        pthread_barrier_wait(&order_barrier);
        pthread_mutex_lock(&mutex);
        printf("Step %d Done by Thread %d\n", steps_count, id);
        steps_count++;
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}

int main()
{
    printf("Production Line\n");
    pthread_barrier_init(&barrier, NULL, EMPLOYEE);
    pthread_barrier_init(&order_barrier, NULL, EMPLOYEE);
    // create 10 threads of EMPLOYEE
    for (int i = 0; i < EMPLOYEE; i++)
    {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&employees[i], NULL, routine, arg);
    }

    // wait for all threads to finish
    for (int i = 0; i < EMPLOYEE; i++)
    {
        pthread_join(employees[i], NULL);
    }
    pthread_barrier_destroy(&barrier);
    pthread_barrier_destroy(&order_barrier);
    return 0;
}