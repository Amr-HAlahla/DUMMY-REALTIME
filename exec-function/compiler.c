#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <source_file.c> <output_name>\n", argv[0]);
        return 1;
    }

    // Extracting the source file name and output name
    char *source_file = argv[1];
    char *output_name = argv[2];

    // Using execlp to call gcc
    if (execlp("gcc", "gcc", source_file, "-o", output_name, NULL) == -1)
    {
        perror("execlp");
        return 1;
    }

    return 0; // This line will never be reached if execlp is successful
}
