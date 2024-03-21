#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 200 // buffer size to be used for read and write the string through pipe

int main(int argc, char *argv[])
{

    int fd[2];
    pid_t pid;

    if (argc != 2)
    {
        printf("Usage: %s <string>\n", argv[0]);
        exit(1);
    }

    if (pipe(fd) == -1)
    {
        perror("pipe failed\n");
        exit(1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed\n");
        exit(1);
    }

    if (pid == 0)
    {
        /*close(fd[0]);       // close the read end of the pipe in the child process
        char buf[BUF_SIZE]; // buffer to read the string from the user
        printf("Input a string: \n");
        fgets(buf, BUF_SIZE, stdin); // read the string from the user
        buf[strlen(buf) - 1] = '\0'; // remove the newline character from the string
        write(fd[1], buf, BUF_SIZE); // write the string to the pipe
        close(fd[1]);                // close the write end of the pipe in the child process
        */
        // the string is passed as an argument to the child process
        close(fd[0]);                              // close the read end of the pipe in the child process
        if (write(fd[1], argv[1], BUF_SIZE) == -1) // write the string to the pipe
        {
            perror("write failed\n");
            exit(1);
        }
        close(fd[0]); // close the read end of the pipe in the child process
    }
    else
    {
        wait(NULL);         // wait for the child process to complete (to read the string from the pipe)
        char buf[BUF_SIZE]; // buffer to read the string from the pipe
        close(fd[1]);       // close the write end of the pipe in the parent process
        if (read(fd[0], buf, BUF_SIZE) == -1)
        { // read the string from the pipe
            perror("read failed\n");
            exit(1);
        }

        printf("The string read from the pipe is: %s\n", buf);
        close(fd[0]); // close the read end of the pipe in the parent process
    }
    return 0;
}
