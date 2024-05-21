#include "threads.h"

void *functionC();

/*Mutexes are used to prevent data inconsistencies due to race conditions.*/
// pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1;
int counter = 0;

int main(int argc, char *argv[])
{
    int rc1, rc2;
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex1, NULL);

    if ((rc1 = pthread_create(&thread1, NULL, &functionC, NULL)))
    {
        printf("Thread creation failed: %d\n", rc1);
    }
    if ((rc2 = pthread_create(&thread2, NULL, &functionC, NULL)))
    {
        printf("Thread creation failed: %d\n", rc2);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&mutex1);

    exit(0);
}

void *functionC()
{
    pthread_mutex_lock(&mutex1);
    counter++;
    printf("Counter value: %d\n", counter);
    pthread_mutex_unlock(&mutex1);
}