#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int p1[2]; // from child to parent
    int p2[2]; // from parent to child

    if (pipe(p1) == -1 || pipe(p2) == -1)
    {
        perror("pipe");
        return 1;
    }

    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(p1[0]); // close read end
        close(p2[1]); // close write end
        int x;
        if (read(p2[0], &x, sizeof(x)) == -1)
        {
            perror("read");
            return 1;
        }
        printf("Child received %d\n", x);
        x = x * 4;
        if (write(p1[1], &x, sizeof(x)) == -1)
        {
            perror("write");
            return 1;
        }
        printf("Child sent %d\n", x);
        close(p1[1]);
        close(p2[0]);
    }
    else
    {
        close(p1[1]); // close write end
        close(p2[0]); // close read end
        srand(time(NULL));
        int y = rand() % 10;
        printf("Parent Generated %d\n", y);
        if (write(p2[1], &y, sizeof(y)) == -1)
        {
            perror("write");
            return 1;
        }
        printf("parent sent %d\n", y);
        if (read(p1[0], &y, sizeof(y)) == -1)
        {
            perror("read");
            return 1;
        }
        printf("Parent Received: %d\n", y);
        close(p1[0]);
        close(p2[1]);
        wait(NULL);
    }
    return 0;
}
