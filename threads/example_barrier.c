#include "threads.h"

/*
    In this example, we have NTHREADS threads that will roll a dice and then check if they are the winner.
    The winner is the thread that rolled the highest value.
    The threads will wait for each other to roll the dice and then check the winner.
    The winner thread will print "I Won!" and the other threads will print "I Lost!".
*/

#define NTHREADS 8

int diceValues[NTHREADS];
int status[NTHREADS] = {0};
pthread_barrier_t diceBarrier;
pthread_barrier_t winnerBarrier;

void *rollDice(void *arg)
{
    while (1)
    {
        int index = *(int *)arg;
        diceValues[index] = rand() % 6 + 1;
        pthread_barrier_wait(&diceBarrier);
        pthread_barrier_wait(&winnerBarrier);
        if (status[index] == 1)
        {
            printf("(%d thread rolled %d) I Won!\n", index, diceValues[index]);
        }
        else
        {
            printf("(%d thread rolled %d) I Lost!\n", index, diceValues[index]);
        }
        sleep(2);
    }
    free(arg);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    pthread_barrier_init(&diceBarrier, NULL, NTHREADS + 1);
    pthread_barrier_init(&winnerBarrier, NULL, NTHREADS + 1);
    srand(time(NULL));
    for (int i = 0; i < NTHREADS; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        pthread_create(&threads[i], NULL, rollDice, index);
    }
    while (1)
    {
        pthread_barrier_wait(&diceBarrier); // guarantee all threads have rolled the dice before checking the winner
        // calculate the winner
        int max = 0;
        for (int i = 0; i < NTHREADS; i++)
        {
            if (diceValues[i] > max)
            {
                max = diceValues[i];
            }
        }
        for (int i = 0; i < NTHREADS; i++)
        {
            if (diceValues[i] == max)
            {
                status[i] = 1;
            }
            else
            {
                status[i] = 0;
            }
        }
        printf("============ New round! =============\n");
        pthread_barrier_wait(&winnerBarrier); // guarantee all threads have checked the winner before printing the result
    }
    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_barrier_destroy(&diceBarrier);
    pthread_barrier_destroy(&winnerBarrier);
    return 0;
}