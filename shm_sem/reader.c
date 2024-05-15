#include "header.h"

// Semaphore operation struct for semop
struct sembuf operation;

int main()
{
    key_t key = ftok(".", 65); // Get the key for shared memory and semaphore

    // Get shared memory
    int shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    char *shm = (char *)shmat(shmid, NULL, 0);
    if (shm == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Get semaphore
    int semid = semget(key, 1, 0666);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }

    while (1)
    {
        // Wait for writer to notify by blocking on semaphore
        operation.sem_num = 0;
        operation.sem_op = -1;
        operation.sem_flg = 0;
        if (semop(semid, &operation, 1) < 0)
        {
            perror("semop");
            exit(1);
        }

        // Read data from shared memory
        printf("Received: %s\n", shm);

        // Check for termination condition
        if (strncmp(shm, "END", 3) == 0)
        {
            break;
        }
    }

    // Detach shared memory
    if (shmdt(shm) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
