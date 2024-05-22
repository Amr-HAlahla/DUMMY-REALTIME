#include "includes.h"
#include "functions.c"

/* Global variables for the shared memory and semaphore */
int shmid;
void *shm;
sem_t *sem;

void openResources();
void closeResources();
void handleSignal(int);

pid_t pid;

int main(int argc, char *argv[])
{
    openResources();
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        execl("./child", "child", NULL);
        perror("execl");
        exit(1);
    }
    sleep(2);
    // Set up signal handler
    if (sigset(SIGUSR1, handleSignal) == SIG_ERR)
    {
        perror("sigset");
        exit(1);
    }
    kill(pid, SIGUSR1);
    // Wait for signals
    while (1)
    {
        pause();
    }

    closeResources();

    return 0;
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

void handleSignal(int signal)
{
    if (signal == SIGUSR1)
    {
        lockSEM(sem);
        printf("Read: %s\n", (char *)shm);
        unlockSEM(sem);
        if (((char *)shm)[strlen((char *)shm) - 1] == 'q')
        {
            closeResources();
            exit(0);
        }
        kill(pid, SIGUSR1);
    }
}