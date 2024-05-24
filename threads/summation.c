#include "threads.h"

/* Problem: calculate the summation of an array of integers using threads
 */
#define THREADS 2
#define SIZE 10
int numbers[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

void *sum_array(void *arg)
{
    int index = *(int *)arg;
    int sum = 0;
    for (int i = 0; i < SIZE / THREADS; i++)
    {
        sum += numbers[index + i];
    }
    *(int *)arg = sum;
    printf("Thread %d: %d\n", index, sum);
    return arg;
}

int main()
{
    pthread_t threads[THREADS];
    int sum = 0;
    int *partial_sum = (int *)malloc(sizeof(int) * THREADS);

    for (int i = 0; i < THREADS; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * (SIZE / THREADS); // start index of the thread
        partial_sum[i] = 0;        // initialize partial sum to 0 for each thread
        pthread_create(&threads[i], NULL, sum_array, a);
    }

    for (int i = 0; i < THREADS; i++)
    {
        void *return_val;
        pthread_join(threads[i], &return_val);
        partial_sum[i] = *(int *)return_val;
        sum += partial_sum[i];
        free(return_val);
    }

    printf("Sum: %d\n", sum);
    free(partial_sum);
    return 0;
}