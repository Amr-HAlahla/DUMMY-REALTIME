#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define PROCESSES_NUM 3

int main(int argc, char *argv[])
{
    int pids[PROCESSES_NUM];
    int pipes[PROCESSES_NUM + 1][2];
    int i;

    for (i = 0; i < PROCESSES_NUM + 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("Error creating pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < PROCESSES_NUM; i++)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("Error creating process");
            exit(EXIT_FAILURE);
        }
        if (pids[i] == 0)
        {
            int j;
            for (j = 0; j < PROCESSES_NUM + 1; j++)
            {
                if (j != i && close(pipes[j][0]) == -1)
                {
                    perror("Error closing pipe read end in child process");
                    exit(EXIT_FAILURE);
                }
                if (j != i + 1 && close(pipes[j][1]) == -1)
                {
                    perror("Error closing pipe write end in child process");
                    exit(EXIT_FAILURE);
                }
            }
            int num;
            if (read(pipes[i][0], &num, sizeof(int)) == -1)
            {
                perror("Error reading from pipe in child process");
                exit(EXIT_FAILURE);
            }
            printf("P%d: done reading %d\n", i + 1, num);
            num++;
            if (write(pipes[i + 1][1], &num, sizeof(int)) == -1)
            {
                perror("Error writing to pipe in child process");
                exit(EXIT_FAILURE);
            }
            printf("P%d: done writing %d\n", i + 1, num);
            if (close(pipes[i][0]) == -1 || close(pipes[i + 1][1]) == -1)
            {
                perror("Error closing pipes in child process");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }

    int x;
    srand(getpid());
    x = rand() % 10 + 1;
    printf("Main generated a random number: %d\n", x);
    if (write(pipes[0][1], &x, sizeof(int)) == -1)
    {
        perror("Error writing to pipe in main process");
        exit(EXIT_FAILURE);
    }
    printf("Main: done writing %d\n", x);

    for (i = 0; i < PROCESSES_NUM; i++)
    {
        wait(NULL);
    }
    if (read(pipes[PROCESSES_NUM][0], &x, sizeof(int)) == -1)
    {
        perror("Error reading from pipe in main process");
        exit(EXIT_FAILURE);
    }
    printf("Main: done reading %d\n", x);

    if (close(pipes[0][1]) == -1 || close(pipes[PROCESSES_NUM][0]) == -1)
    {
        perror("Error closing pipes in main process");
        exit(EXIT_FAILURE);
    }

    printf("Main: done\n");
    return EXIT_SUCCESS;
}
