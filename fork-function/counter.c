#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *args[])
{

    int pid = fork();
    int n;

    if (pid == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }
    if (pid != 0)
    {
        wait(NULL);
    }

    for (int i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }

    return 0;
}
