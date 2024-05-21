/*
A condition variable (of type pthread_cond_t) is used for waiting until a particular condition is true.
It allows threads to suspend execution and give up the processor until the condition is met.

To avoid race conditions, a condition variable should always be associated with a mutex.
This prevents a situation where a thread is preparing to wait and another thread signals the condition before the first thread actually waits, leading to a deadlock.

Functions associated with condition variables include:

- Creation/Destruction:
  - pthread_cond_init
  - pthread_cond_t cond = PTHREAD_COND_INITIALIZER
  - pthread_cond_destroy

- Waiting on condition:
  - pthread_cond_wait
  - pthread_cond_timedwait (with a limit on how long it will block)

- Waking thread based on condition:
  - pthread_cond_signal
  - pthread_cond_broadcast (wakes up all threads blocked by the specified condition variable)
*/

#include "threads.h"

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;

void *functionCount1();
void *functionCount2();

int count = 0;

#define COUNT_DONE 10
#define COUNT_HALT1 3
#define COUNT_HALT2 6

int main()
{
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, &functionCount1, NULL);
    pthread_create(&thread2, NULL, &functionCount2, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    exit(0);
}

void *functionCount1()
{
    for (;;)
    {
        pthread_mutex_lock(&condition_mutex);
        if (count >= COUNT_HALT1 && count <= COUNT_HALT2) // If count is between COUNT_HALT1 and COUNT_HALT2
        {
            pthread_cond_wait(&condition_cond, &condition_mutex); // Wait for the condition
        }
        pthread_mutex_unlock(&condition_mutex);

        pthread_mutex_lock(&count_mutex);
        count++;
        printf("Counter value functionCount1: %d\n", count);
        pthread_mutex_unlock(&count_mutex);

        if (count >= COUNT_DONE)
            return (NULL);
    }
}

void *functionCount2()
{
    for (;;)
    {
        pthread_mutex_lock(&condition_mutex);
        if (count < COUNT_HALT1 || count > COUNT_HALT2) // If count is less than COUNT_HALT1 or greater than COUNT_HALT2
        {
            pthread_cond_signal(&condition_cond); // Signal the condition
        }
        pthread_mutex_unlock(&condition_mutex);

        pthread_mutex_lock(&count_mutex);
        count++;
        printf("Counter value functionCount2: %d\n", count);
        pthread_mutex_unlock(&count_mutex);

        if (count >= COUNT_DONE)
            return (NULL);
    }
}