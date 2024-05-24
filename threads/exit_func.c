#include "threads.h"

void *roll_dice()
{
    int *value = malloc(sizeof(int));
    /* why using allocated memory?
     * The memory allocated here will be used in the main function
     * and we need to return the value of the dice roll to the main function
     * if we don't allocate memory here, the value will be lost when the function ends
     */
    *value = rand() % 6 + 1;
    pthread_exit((void *)value);
}

int main()
{
    int *result;
    srand(time(NULL));

    pthread_t thread1;
    if (pthread_create(&thread1, NULL, &roll_dice, NULL) != 0)
    {
        perror("pthread_create");
        exit(1);
    }
    /* getting the result from the thread from its return value */
    if (pthread_join(thread1, (void **)&result) != 0)
    {
        perror("pthread_join");
        exit(1);
    }
    // pthread_exit(0);
    /*
        pthread_exit(0);
        calling will not terminate the main thread directly,
        it will wait for all the threads to finish their execution and then terminate the main thread
    */
    printf("Result: %d\n", *result);
    free(result); // free the allocated memory
    return 0;
}