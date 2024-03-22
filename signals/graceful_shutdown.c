#include "local.h"

void graceful_exit(int signo)
{
    printf("\tChild received signal %d\n", signo);
    sleep(1);
    printf("\tChild about to terminate gracefully...\n");
    sleep(2);
    exit(0);
}

void child_code()
{

    struct sigaction act;
    sigemptyset(&act.sa_mask); // clear the mask, to allow all signals
    act.sa_flags = SA_RESTART; // no flags
    act.sa_handler = graceful_exit;
    if (-1 == sigaction(SIGTERM, &act, NULL))
    {
        perror("sigaction");
        exit(1);
    }

    printf("Child running\n");
    while (1)
    {
        printf("\tChild just woke up, but going back to sleep.\n");
        sleep(1);
    }
}

void parent_code(pid_t pid)
{
    printf("Parent sleeping for 5 seconds\n");
    sleep(5);

    // try to terminate the child!!
    printf("Parent about to kill child\n");
    if (-1 == kill(pid, SIGTERM))
    {
        perror("kill failed");
        exit(1);
    }
    wait(NULL);
    printf("My child terminated, about to exit myself ...\n");
}

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        child_code();
    }
    else
    {
        parent_code(pid);
    }
}
