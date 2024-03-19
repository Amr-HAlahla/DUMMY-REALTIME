#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int x = 0;
void handler_sigusr1(int sig)
{
    if (x == 0)
    {
        printf("\nHint: 10 + 5\n");
    }
}
void handler_sigusr2(int sig)
{
    printf("Terminating Child Process\n");
    exit(0);
}

int main(int argc, char *argv[])
{

    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        // Child Process
        // sigset(SIGUSR2, handler_sigusr2);
        sleep(5);
        kill(getppid(), SIGUSR1);
    }
    else
    {
        // Parent Process
        struct sigaction sa;
        sa.sa_handler = handler_sigusr1;
        sa.sa_flags = SA_RESTART; // Restart system calls
        sigemptyset(&sa.sa_mask);

        // sigset(SIGUSR1, handler_sigusr1);
        if (sigaction(SIGUSR1, &sa, NULL) == -1)
        {
            perror("sigaction");
            exit(1);
        }

        printf("What is the result of 3 * 5 : ");
        scanf("%d", &x);
        if (x == 15)
        {
            printf("Correct\n");
            // kill(pid, SIGUSR2); // Send SIGUSR2 to child process to terminate
        }
        else
        {
            printf("Wrong\n");
        }
        wait(NULL);
    }
}
