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
    // printf("Rolled a %d\n", *value);
    return (void *)value;
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

    if (pthread_join(thread1, (void **)&result) != 0)
    {
        perror("pthread_join");
        exit(1);
    }

    printf("Result: %d\n", *result);
    free(result); // free the allocated memory
    return 0;
}