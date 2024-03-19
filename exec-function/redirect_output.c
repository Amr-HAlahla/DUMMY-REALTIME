#include <stdio.h>    // for printf, perror
#include <unistd.h>   // for fork, execlp
#include <stdlib.h>   // for exit
#include <sys/wait.h> // for wait
#include <fcntl.h>    // for open flags

int main(int argc, char *argv[])
{
    // read the website name as an argument
    if (argc != 2)
    {
        printf("Usage: %s <website>\n", argv[0]);
        exit(1);
    }

    char *website = argv[1];

    int pid = fork();

    if (pid == -1)
    {
        perror("Error fork");
        exit(2);
    }

    if (pid == 0)
    {

        // open a file to redirect the output of the ping command
        int file = open("ping_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777); // TRUNC to clear the file if it exists
        if (file == -1)
        {
            perror("Error open");
            exit(4);
        }

        printf("The file descriptor of the ping result is: %d\n", file);
        // int file2 = dup2(file, 1);               // redirect the standard output to the file
        int file2 = dup2(file, STDOUT_FILENO); // redirect the standard output to the file
        printf("The duplicated file descriptor is: %d\n", file2);
        close(file);                             // close the file descriptor
        printf("Hello! I am a child process\n"); // this will be written to the file

        // replace the current process with the ping command using execlp.
        execlp("ping", "ping", "-c", "4", website, NULL);
        perror("Error execlp");
        exit(3);
    }
    else
    {
        wait(NULL);
        printf("Success!\n"); // this will be written to the terminal
    }
}
