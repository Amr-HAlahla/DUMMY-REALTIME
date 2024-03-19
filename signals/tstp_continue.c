#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handler_sigtstp(int signal)
{
    // stop the process
    printf("stopped\n");
    fflush(stdout);
    raise(SIGSTOP);
}

void handler_sigcont(int signal)
{
    // continue the process
    printf("continued\n");
    printf("enter a number: ");
    fflush(stdout);
}
int main(int argc, char *argv[])
{
    // define the action to take on receipt of SIGTSTP
    struct sigaction sa;
    sa.sa_handler = handler_sigtstp;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa, NULL);

    // define the action to take on receipt of SIGCONT
    struct sigaction sa2;
    sa2.sa_handler = handler_sigcont;
    sa2.sa_flags = SA_RESTART;
    sigaction(SIGCONT, &sa2, NULL);

    // signal(SIGTSTP, signal_handler);

    int x;
    printf("enter a number: ");
    scanf("%d", &x);
    printf("You entered: %d\n", x);
    return 0;
}
