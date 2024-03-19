#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> // for open and O_WRONLY and O_RDONLY

int main(int argc, char *argv[])
{

    int fd = open("sumfifo", O_RDONLY);

    if (fd == -1)
    {
        perror("Error openeing the file\n");
        exit(1);
    }

    int arr[5];
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        if (read(fd, &arr[i], sizeof(int)) == -1)
        {
            perror("Error reading from the file\n");
            exit(1);
        }
        printf("Received number: %d\n", arr[i]);
        sum += arr[i];
    }

    printf("Sum of the numbers: %d\n", sum);
    close(fd);
    return 0;
}
