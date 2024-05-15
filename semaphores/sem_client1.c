#include "semaphore.h"

int main()
{
    int sem_id = semget(SEM_KEY, 2, 0);
    if (sem_id == -1)
    {
        perror("Failed to access semaphore");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        signal_semaphore(sem_id, SEM_CLIENT1); // Signal Server
        printf("Sent signal to Server from Client 1\n");
        wait_semaphore(sem_id, SEM_CLIENT2); // Wait for Server
    }

    return 0;
}
