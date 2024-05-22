#include "includes.h"
#include "functions.c"

/* Global variables for the shared memory and semaphore */
int shmid;
void *shm;
sem_t *sem;

void openResources();
void closeResources();
void signalHandler(int signum);

int main(int argc, char *argv[])
{
    openResources();
    printf("Child process started\n");
    // set the usr 1 signal
    if (sigset(SIGUSR1, signalHandler) == SIG_ERR)
    {
        perror("sigsent");
        exit(1);
    }
    while (1)
    {
        pause();
    }
    closeResources();

    return 0;
}

void signalHandler(int signum)
{
    if (signum == SIGUSR1)
    {
        char input[SHM_SIZE];
        lockSEM(sem);
        printf("Enter any string, or 'q' to quit: ");
        fgets(input, SHM_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0'; // remove trailing newline
        strcpy((char *)shm, input);
        unlockSEM(sem);

        // Send signal to parent process
        kill(getppid(), SIGUSR1);

        if (strcmp(input, "q") == 0)
        {
            closeResources();
            exit(0);
        }
    }
}

void openResources()
{
    shmid = openSHM(SHM_NAME, SHM_SIZE);
    shm = mapSHM(shmid, SHM_SIZE);
    sem = openSEM(SEM_NAME, 1);

    printf("Resources opened\n");
}

void closeResources()
{
    closeSHM(shm, SHM_SIZE);
    sem_close(sem);
    printf("Resources closed\n");
}