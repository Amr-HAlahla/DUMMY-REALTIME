#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define THREADS 4
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *routine(void *arg)
{
    int *index = (int *)arg;
    printf("Thread %d waiting for barrier\n", *index);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    printf("Thread %d passed the barrier\n", *index);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[THREADS];
    for (int i = 0; i < THREADS; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&threads[i], NULL, &routine, index) != 0)
        {
            perror("pthread_create");
            return 1;
        }
    }
    sleep(1);
    printf("Barrier opened\n");
    pthread_cond_broadcast(&cond);
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
}