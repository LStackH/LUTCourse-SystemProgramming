#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // If no files are given, exit with status 0
    if (argc == 1)
    {
        exit(0);
    }

    // Go through all the given files
    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        if (!file)
        {
            // if file cant be accessed
            fprintf(stderr, "my-cat: cannot open file\n");
            exit(1);
        }

        // buffer to hold the line
        char buffer[1024];
        // Read and print the file line by line
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            printf("%s", buffer);
        }

        fclose(file);
    }

    return 0;
}