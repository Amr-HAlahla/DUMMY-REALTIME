#include "includes.h"
#include "functions.c"

#define LINES 1
#define EMPLOYEE 10

pid_t lines[LINES];

int main()
{
    // create 10 processes of LINES
    for (int i = 0; i < LINES; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            execl("./line", "line", NULL);
        }
        else
        {
            // parent process
            printf("Parent process %d created\n", getpid());
            lines[i] = pid;
        }
    }

    // wait for all processes to finish
    for (int i = 0; i < LINES; i++)
    {
        waitpid(lines[i], NULL, 0);
    }

    printf("Parent process %d finished\n", getpid());
    return 0;
}
