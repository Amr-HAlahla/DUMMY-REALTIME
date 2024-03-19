#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> // for open and O_WRONLY and O_RDONLY

int main(int argc, char *argv[])
{

    int fd = open("sumfifo", O_WRONLY);

    if (fd == -1)
    {
        perror("Error openeing the file\n");
        exit(1);
    }

    int arr[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++)
    {
        if (write(fd, &arr[i], sizeof(int)) == -1)
        {
            perror("Error writing to the file\n");
            exit(2);
        }
        printf("Wrote %d to the file\n", arr[i]);
        sleep(1);
    }
    /*
    if(write(fd, arr, sizeof(arr)) == -1){
        perror("Error writing to the file\n");
        exit(2);
    }
    */

    close(fd);
    return 0;
}
