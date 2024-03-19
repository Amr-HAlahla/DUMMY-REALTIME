#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> // for open and O_WRONLY and O_RDONLY

int main(int argc, char *argv[])
{
    if (mkfifo("myfifo", 0777) == -1)
    {
        if (errno != EEXIST) // if the error is not that the file already exists
        {
            perror("Error creating the named pipe");
            exit(1);
        }
    }

    int fd = open("myfifo", O_WRONLY); // open the file for writing
    if (fd == -1)
    {
        perror("Error opening the named pipe");
        exit(2);
    }

    int x = 120;
    if (write(fd, &x, sizeof(x)) == -1) // write to the file
    {
        perror("Error writing to the named pipe");
        exit(3);
    }

    close(fd); // we are done writing to the file so close it

    return 0;
}
