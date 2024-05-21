#include "threads.h"

#define NUM_STOVES 4
#define NUM_CHEFS 10

/*
    4 stoves, 10 chefs
    Each chef needs a random amount of fuel to cook
    Each stove has 100 fuel
    If a chef needs more fuel than what's left in the stove, they go home
    If a stove is being used, the chef waits for it to be available
*/

pthread_mutex_t stoveMutex[NUM_STOVES];
int stoveFuel[NUM_STOVES] = {100, 100, 100, 100};

void *routine(void *args)
{
    for (int i = 0; i < NUM_STOVES; i++)
    {
        if (pthread_mutex_trylock(&stoveMutex[i]) == 0)
        {
            int fuelNeeded = (rand() % 30);
            if (stoveFuel[i] - fuelNeeded < 0)
            {
                printf("No more fuel... going home\n");
            }
            else
            {
                stoveFuel[i] -= fuelNeeded;
                usleep(500000);
                printf("Fuel left in stove %d: %d\n", i, stoveFuel[i]);
            }
            pthread_mutex_unlock(&stoveMutex[i]);
            break;
        }
        else
        {
            if (i == NUM_STOVES - 1)
            {
                printf("No stove available yet, waiting...\n");
                usleep(300000);
                i = -1; // reset to -1 because the loop increment will make it 0
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t th[NUM_CHEFS];
    for (int i = 0; i < NUM_STOVES; i++)
    {
        pthread_mutex_init(&stoveMutex[i], NULL);
    }
    for (int i = 0; i < NUM_CHEFS; i++)
    {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
        {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < NUM_CHEFS; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    for (int i = 0; i < NUM_STOVES; i++)
    {
        pthread_mutex_destroy(&stoveMutex[i]);
    }
    return 0;
}