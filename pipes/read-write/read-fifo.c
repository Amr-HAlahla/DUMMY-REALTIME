#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h> // for open and O_WRONLY and O_RDONLY

int main(int argc, char *argv[])
{

    if (mkfifo("myfifo", 0777) == -1) // create the named pipe with the given permissions
    {
        if (errno != EEXIST) // if the error is not that the file already exists
        {
            perror("Error creating the named pipe");
            exit(1);
        }
    }

    int fd = open("myfifo", O_RDONLY); // open the file for reading
    if (fd == -1)
    {
        perror("Error opening the named pipe");
        exit(4);
    }
    int y;
    if (read(fd, &y, sizeof(y)) == -1) // read from the file
    {
        perror("Error reading from the named pipe");
        exit(5);
    }
    printf("Read from the named pipe: %d\n", y); // print the read value
    close(fd);                                   // we are done reading from the file so close it
    return 0;
}
