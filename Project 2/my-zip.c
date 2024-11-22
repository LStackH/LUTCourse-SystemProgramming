#include <stdio.h>
#include <stdlib.h>

void compressFile(FILE *file)
{
    int count = 0;
    char current, previous;

    // while not end of file, go through it character by character
    while ((current = fgetc(file)) != EOF)
    {
        // same characters increase count
        if (current == previous)
        {
            count++;
        }
        // character differs, write current count and corresponding character
        else
        {
            if (count > 0)
            {
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&previous, sizeof(char), 1, stdout);
            }
            // reset count
            count = 1;
            previous = current;
        }
    }

    // final run, if at least one character
    if (count > 0)
    {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&previous, sizeof(char), 1, stdout);
    }
}

int main(int argc, char *argv[])
{
    // no files given
    if (argc < 2)
    {
        fprintf(stderr, "my-zip: file1 [file2 ...]\n");
        exit(1);
    }

    // go through each file
    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        if (!file)
        {
            fprintf(stderr, "my-zip: cannot open file\n");
            exit(1);
        }
        compressFile(file);
        fclose(file);
    }

    return 0;
}

// used for help: https://stackoverflow.com/questions/51055250/rle-algorithm-should-compress-bytes-in-c
