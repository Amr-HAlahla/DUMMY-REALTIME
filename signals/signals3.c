#include "local.h"

void sighub(int signo);
void sigint(int signo);
void sigquit(int signo);

int main(int argc, char *argv[])
{

    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        signal(SIGHUP, sighub);   // catch hangup signal
        signal(SIGINT, sigint);   // catch interrupt signal
        signal(SIGQUIT, sigquit); // catch quit signal
        for (;;)
            ; // loop for ever
    }
    else
    {
        printf("\nPARENT: sending SIGHUP\n\n");
        kill(pid, SIGHUP);
        sleep(3); /* pause for 3 seconds */
        printf("\nPARENT: sending SIGINT\n\n");
        kill(pid, SIGINT);
        sleep(3); /* pause for 3 seconds */
        printf("\nPARENT: sending SIGQUIT\n\n");
        kill(pid, SIGQUIT);
        sleep(3);
    }
}

void sighub(int signo)
{
    signal(SIGHUP, sighub); /* reset signal */
    printf("CHILD: I have received a SIGHUP\n");
}

void sigint(int signo)
{
    signal(SIGINT, sigint); /* reset signal */
    printf("CHILD: I have received a SIGINT\n");
}

void sigquit(int signo)
{
    printf("My DADDY has Killed me!!!\n");
    exit(0);
}
