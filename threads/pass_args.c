#include "threads.h"

/* The Problem
-   create 10 threads. each taking a unique prime from the primes array and printing it.
*/
#define THREADS 10
int primes[THREADS] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void *print_prime(void *arg)
{
    // int *prime = (int *)arg;
    // printf("%d\n", *prime);
    int *prime = (int *)arg;
    printf("%d\n", primes[*prime]);
    return NULL;
}

int main()
{
    pthread_t threads[THREADS];
    // for (int i = 0; i < THREADS; i++)
    // {
    //     if (pthread_create(&threads[i], NULL, &print_prime, &primes[i]) != 0)
    //     {
    //         perror("pthread_create");
    //         exit(1);
    //     }
    // }

    for (int i = 0; i < THREADS; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&threads[i], NULL, &print_prime, a) != 0)
        {
            perror("pthread_create");
            exit(1);
        }
    }

    for (int i = 0; i < THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(1);
        }
    }

    return 0;
}