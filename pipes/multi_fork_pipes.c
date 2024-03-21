#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{

    /*
    program to create 3 processes and 3 pipes to communicate between them,
    where each process sends a number to the next process in a circular manner (P1 -> P2 -> P3 -> P1)
    */
    int fd[3][2]; // 3 pipes for 3 processes
    /*
    first pipe => fd[0] from process 1 to process 2 (P1 writes and P2 reads)
    second pipe => fd[1] from process 2 to process 3 (P2 writes and P3 reads)
    third pipe => fd[2] from process 3 to process 1 (P3 writes and P1 reads)
    */
    int i;
    for (i = 0; i < 3; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Error creating pipe");
            exit(1);
        }
    }

    pid_t pid[3]; // 3 processes
    for (i = 0; i < 3; i++)
    {
        pid[i] = fork();
        if (pid[i] < 0)
        {
            perror("Error creating process");
            exit(1);
        }
        if (pid[i] == 0)
        {
            break;
        }
    }

    if (pid[0] == 0)
    {
        close(fd[0][0]); // close read end of pipe 1
        close(fd[2][1]); // close write end of pipe 3
        // get a random number between 1 and 100
        srand(getpid());
        int num = rand() % 10 + 1;
        printf("P1: %d\n", num);

        // write the number to the next process
        if (write(fd[0][1], &num, sizeof(int)) == -1)
        {
            perror("Error writing to pipe");
            exit(1);
        }
        close(fd[0][1]); // close write end of pipe 1

        // read the number from the previous process
        if (read(fd[2][0], &num, sizeof(int)) == -1)
        {
            perror("Error reading from pipe");
            exit(1);
        }
        printf("P1: Received number: %d\n", num);

        close(fd[2][0]); // close read end of pipe 3
        printf("P1: Done\n");
    }
    else if (pid[1] == 0)
    {
        close(fd[1][0]); // close read end of pipe 2
        close(fd[0][1]); // close write end of pipe 1
        int num;
        // read the number from the previous process
        if (read(fd[0][0], &num, sizeof(int)) == -1)
        {
            perror("Error reading from pipe");
            exit(1);
        }
        // do some processing on the number
        num *= 2;
        printf("P2: Number multiplied by 2: %d\n", num);
        // write the number to the next process
        if (write(fd[1][1], &num, sizeof(int)) == -1)
        {
            perror("Error writing to pipe");
            exit(1);
        }
        close(fd[1][1]); // close write end of pipe 2
        close(fd[0][0]); // close read end of pipe 1
        printf("P2: Done\n");
    }
    else if (pid[2] == 0)
    {
        close(fd[2][0]); // close read end of pipe 3
        close(fd[1][1]); // close write end of pipe 2
        int num;
        // read the number from the previous process
        if (read(fd[1][0], &num, sizeof(int)) == -1)
        {
            perror("Error reading from pipe");
            exit(1);
        }
        // do some processing on the number
        num += 1;
        printf("P3: Number incremented by 1: %d\n", num);
        // write the number to the next process
        if (write(fd[2][1], &num, sizeof(int)) == -1)
        {
            perror("Error writing to pipe");
            exit(1);
        }
        close(fd[2][1]); // close write end of pipe 3
        close(fd[1][0]); // close read end of pipe 2
        printf("P3: Done\n");
    }
    else
    {
        // close all ends of the pipes in the parent process
        for (i = 0; i < 3; i++)
        {
            close(fd[i][0]);
            close(fd[i][1]);
        }
        // wait for all child processes to finish
        for (i = 0; i < 3; i++)
        {
            waitpid(pid[i], NULL, 0);
        }
    }
    return 0;
}
