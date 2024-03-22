#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define NUM_CHILDREN 3
#define BUFFER_SIZE 100

int main()
{
    int fd[NUM_CHILDREN][2];

    pid_t pids[NUM_CHILDREN];
    int i;

    for (i = 0; i < NUM_CHILDREN; i++)
    {
        if (pipe(fd[i]) < 0)
        {
            // Handle error
            printf("Could not fork child %d\n", i);
            exit(-1 * i);
        }

        // fcntl(fd[i],
        pids[i] = fork();

        if (pids[i] < 0)
        {
            // Handle error
            printf("Could not fork child %d\n", i);
            exit(-10 * i);
        }

        if (pids[i] == 0)
        {
            // Child process
            close(fd[i][0]); // Close read end
            char message[BUFFER_SIZE];
            sprintf(message, "Hello from child %d!", i + 1);

            // sleep(10);

            // if ( i != 0 )
            write(fd[i][1], message, strlen(message)); // Write to pipe
            close(fd[i][1]);                           // Close write end

            // while(1);
            return 0;
        }
    }

    // Parent process
    for (i = 0; i < NUM_CHILDREN; i++)
    {
        close(fd[i][1]); // Close write end
        char buffer[BUFFER_SIZE];

        memset(buffer, 0x0, BUFFER_SIZE);

        read(fd[i][0], buffer, BUFFER_SIZE); // Read from pipe
        printf("Parent received message from child %d: %s\n", i + 1, buffer);
        close(fd[i][0]); // Close read end
    }

    return 0;
}
