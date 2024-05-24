#include "threads.h"

/*A join is performed when one wants to wait for a thread to finish.*/
#define THREADS 10
void *thread_function(void *);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main(int argc, char *argv[])
{
    pthread_t thread[THREADS];
    int i, rc;

    for (i = 0; i < THREADS; i++)
    {
        if ((rc = pthread_create(&thread[i], NULL, &thread_function, NULL) != 0))
        {
            printf("Thread creation failed: %d\n", rc);
        }
    }

    for (i = 0; i < THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    printf("Final counter value: %d\n", counter);

    exit(0);
}

void *thread_function(void *ptr)
{
    pthread_mutex_lock(&mutex1);
    printf("Thread number %ld\n", pthread_self());
    counter++;
    pthread_mutex_unlock(&mutex1);
}
