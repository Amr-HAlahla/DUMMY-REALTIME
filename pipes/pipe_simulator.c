#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe failed\n");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork failed\n");
        exit(2);
    }
    if (pid1 == 0)
    {
        // child process 1(execute ping command)
        dup2(fd[1], STDOUT_FILENO); // redirect the standard output to the write end of the pipe (STDOUT_FILENO = 1)
        close(fd[0]);               // close the read end of the pipe in the child process
        close(fd[1]);               // close the write end of the pipe in the child process
        execlp("ping", "ping", "-c", "5", "www.ritaj.birzeit.edu", NULL);
    }
    /* child process 1 will not reach this point, it will be replaced by the ping command,
     so the parent process will continue from here.*/

    pid_t pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork 2 failed\n");
        exit(3);
    }
    if (pid2 == 0)
    {
        // child process 2(execute grep command, grep for rtt)
        dup2(fd[0], STDIN_FILENO); // redirect the standard input to the read end of the pipe (STDIN_FILENO = 0)
        close(fd[1]);
        close(fd[0]);
        execlp("grep", "grep", "rtt", NULL);
    }
    /* child process 2 will not reach this point, it will be replaced by the grep command,
     so the parent process will continue from here.*/
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0); // wait for the child process 1 to complete
    waitpid(pid2, NULL, 0); // wait for the child process 2 to complete
    return 0;
}
