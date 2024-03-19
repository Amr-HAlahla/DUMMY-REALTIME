#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /*
    2 processes
    1- parent will create an array of 10 integers and fill it with random numbers, then send it to the child using pipe
    2- child will sort the array and print it
    */
    int arr[10];
    int i;
    int fd[2];
    int n = 10;

    for (i = 0; i < n; i++)
    {
        arr[i] = rand() % 10;
    }

    if (pipe(fd) == -1)
    {
        printf("Error in pipe\n");
        exit(1);
    }

    int pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork failed");
        exit(1);
        break;

    case 0:
        sleep(1);     // to make sure the parent writes the whole array before the child reads it
        close(fd[1]); // close the write end
        if (read(fd[0], arr, sizeof(int) * n) == -1)
        {
            perror("read failed");
            exit(1);
        }
        printf("Child received the array from the parent\n");
        // sort the array, using bubble sort
        int j;
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        printf("Sorted array: \n");
        for (i = 0; i < n; i++)
        {
            printf("%d \n", arr[i]);
        }
        close(fd[0]); // close the read end
        break;

    default:
        close(fd[0]);                       // close the read end
        write(fd[1], arr, sizeof(int) * n); // write the array to the pipe
        for (i = 0; i < n; i++)
        {
            printf("%d \n", arr[i]);
        }
        printf("Parent sent the array to the child\n");
        close(fd[1]); // close the write end
        wait(NULL);   // wait for the child to finish
        break;
    }

    return 0;
}
