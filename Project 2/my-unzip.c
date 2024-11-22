#include <stdio.h>
#include <stdlib.h>

void decompressFile(FILE *file)
{
    int count;
    char character;

    // read 1 int at a time, store the value
    while (fread(&count, sizeof(int), 1, file) == 1)
    {
        // read one char, store it
        if (fread(&character, sizeof(char), 1, file) != 1)
        {
            fprintf(stderr, "my-unzip: file format error\n");
            exit(1);
        }
        // if read, print the stored char, for 'count' times
        for (int i = 0; i < count; i++)
        {
            putchar(character);
        }
    }
}

int main(int argc, char *argv[])
{
    // no files given
    if (argc < 2)
    {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    // go through each file
    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "rb");
        if (!file)
        {
            fprintf(stderr, "my-unzip: cannot open file\n");
            exit(1);
        }
        decompressFile(file);
        fclose(file);
    }

    return 0;
}
