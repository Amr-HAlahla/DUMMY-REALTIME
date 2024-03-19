#include <stdio.h>    // for printf, perror
#include <unistd.h>   // for fork, execlp
#include <stdlib.h>   // for exit
#include <sys/wait.h> // for wait

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <fifo_name>\n", argv[0]);
        exit(1);
    }

    int pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        execlp("mkfifo", "mkfifo", argv[1], NULL);
        perror("execlp");
        exit(1);
    }
    else
    {
        wait(NULL);
        printf("Success!\n");
    }
    return 0;
}
