#include "semaphore.h"

int main()
{
    int sem_id = semget(SEM_KEY, 2, IPC_CREAT | 0666);
    if (sem_id == -1)
    {
        perror("Failed to create semaphore");
        exit(EXIT_FAILURE);
    }

    union semun sem_union;
    sem_union.val = 1;
    if (semctl(sem_id, SEM_CLIENT1, SETVAL, sem_union) == -1)
    {
        perror("Failed to set semaphore value");
        exit(EXIT_FAILURE);
    }

    sem_union.val = 1;
    if (semctl(sem_id, SEM_CLIENT2, SETVAL, sem_union) == -1)
    {
        perror("Failed to set semaphore value");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore server started.\n");

    while (1)
    {
        // Server waits for signals from either client
        wait_semaphore(sem_id, SEM_CLIENT1);
        printf("Received signal from Client 1\n");
        sleep(1);                              // Simulate processing time
        signal_semaphore(sem_id, SEM_CLIENT2); // Signal Client 2

        wait_semaphore(sem_id, SEM_CLIENT2);
        printf("Received signal from Client 2\n");
        sleep(1);                              // Simulate processing time
        signal_semaphore(sem_id, SEM_CLIENT1); // Signal Client 1
    }

    return 0;
}
