#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{

    int arr[] = {1, 2, 3, 4, 1, 2, 3};
    int start, end;
    int arrSize = sizeof(arr) / sizeof(int);
    int fd[2];

    if (pipe(fd) == -1)
    {
        exit(1);
    }

    int pid = fork();
    if (pid == -1)
    {
        exit(2);
    }

    if (pid == 0)
    {
        start = 0;
        end = arrSize / 2;
    }
    else
    {
        start = arrSize / 2;
        end = arrSize;
    }

    int sum = 0;
    if (pid == 0)
    {
        for (int i = start; i < end; i++)
        {
            sum += arr[i]; // calculate sum of array in child process
        }
        printf("Calculated partial sum by te child: %d\n", sum);
    }
    else
    {
        for (int i = start; i < end; i++)
        {
            sum += arr[i];
        }
        printf("Calculated partial sum by the parent: %d\n", sum);
    }

    if (pid == 0)
    {
        close(fd[0]);                    // close read end
        write(fd[1], &sum, sizeof(int)); // write sum to pipe
        close(fd[1]);
    }
    else
    {
        int sumFromChild;
        close(fd[1]);                            // close write end
        read(fd[0], &sumFromChild, sizeof(int)); // read sum from pipe
        close(fd[0]);                            // close read end
        wait(NULL);                              // Wait for the child process to finish before continuing.

        int totalSum = sum + sumFromChild;
        printf("Total sum: %d\n", totalSum); // print the total sum
    }
    return 0;
}
