#include <stdio.h>    // for printf, perror
#include <unistd.h>   // for fork, execlp
#include <stdlib.h>   // for exit
#include <sys/wait.h> // for wait

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
        // replace the current process with the ping command using execlp.
        execlp("ping", "ping", "-c", "4", website, NULL);
        perror("Error execlp");
        exit(3);
    }
    else
    {
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus)) // check if the child process terminated normally (WIFEXITED macro)
        {
            int statusCode = WEXITSTATUS(wstatus); // get the exit status code of the child process
            if (statusCode == 0)
            {
                printf("Success!\n");
            }
            else
            {
                printf("Failure: %d\n", statusCode);
            }
        }
    }
}
