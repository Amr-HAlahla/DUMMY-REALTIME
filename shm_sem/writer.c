#include "header.h"

// Semaphore operation struct for semop
struct sembuf operation;

int main()
{
    key_t key = ftok(".", 65); // Generate a key for shared memory and semaphore

    // Create shared memory
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
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

    // Create semaphore
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore
    union semun sem_init;
    /* Initial value of the semaphore, 0 indicates that the writer has not written anything yet */
    sem_init.val = 0;
    if (semctl(semid, 0, SETVAL, sem_init) < 0)
    {
        perror("semctl");
        exit(1);
    }

    while (1)
    {
        printf("Enter a message, or type 'END' to terminate: ");
        /* Write data to shared memory */
        fgets(shm, SHM_SIZE, stdin);
        /* Notify reader by incrementing the semaphore value by 1 */
        operation.sem_num = 0; /* Index of the semaphore in the semaphore array */
        operation.sem_op = 1;  /* Increment the semaphore value by 1 */
        operation.sem_flg = 0; /* No flags */
        if (semop(semid, &operation, 1) < 0)
        {
            perror("semop");
            exit(1);
        }

        sleep(1); // Simulate some work

        // Termination condition
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

    // Remove shared memory
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }

    // Remove semaphore
    if (semctl(semid, 0, IPC_RMID, 0) == -1)
    {
        perror("semctl");
        exit(1);
    }

    return 0;
}
