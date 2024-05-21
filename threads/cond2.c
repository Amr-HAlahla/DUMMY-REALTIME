#include "threads.h"

#define THREADS 2
pthread_mutex_t fuel_mutex;
pthread_cond_t fuel_cond;
int fuel = 0;

void *fuel_filling()
{
    printf("Filling fuel\n");
    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&fuel_mutex);
        fuel += 10;
        printf("Fuel: %d\n", fuel);
        pthread_mutex_unlock(&fuel_mutex);
        pthread_cond_signal(&fuel_cond);
        sleep(1);
    }
}
void *car()
{
    printf("Car\n");
    pthread_mutex_lock(&fuel_mutex);
    while (fuel < 50)
    {
        printf("Waiting for fuel ...\n");
        pthread_cond_wait(&fuel_cond, &fuel_mutex);
    }
    printf("Driving\n");
    fuel -= 50;
    printf("Fuel: %d\n", fuel);
    pthread_mutex_unlock(&fuel_mutex);
}

int main()
{

    pthread_t threads[THREADS];
    pthread_mutex_init(&fuel_mutex, NULL);
    pthread_cond_init(&fuel_cond, NULL);

    for (int i = 0; i < THREADS; i++)
    {
        if (i % 2 == 0)
        {
            if (pthread_create(&threads[i], NULL, &car, NULL) != 0)
            {
                perror("pthread_create");
                exit(1);
            }
        }
        else
        {
            if (pthread_create(&threads[i], NULL, &fuel_filling, NULL) != 0)
            {
                perror("pthread_create");
                exit(1);
            }
        }
    }

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&fuel_mutex);
    pthread_cond_destroy(&fuel_cond);
    return 0;
}