#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    char str[50];

    // 2 args, reverse input
    if (argc == 2)
    {
        inputFile = fopen(argv[1], "r");
        if (!inputFile)
        {
            fprintf(stderr, "error: cannot open file %s'\n'", argv[1]);
            exit(1);
        }

        while (fgets(str, 50, inputFile) != NULL)
        {
            fprintf(stdout, "%s", str);
        }
        printf("\n");
        fclose(inputFile);
    }
    // 3 args, reverse input output
    else if (argc == 3)
    {
        inputFile = fopen(argv[1], "r");
        if (!inputFile)
        {
            fprintf(stderr, "error: cannot open file %s'\n'", argv[1]);
            exit(1);
        }
        outputFile = fopen(argv[2], "w");
        if (!outputFile)
        {
            fprintf(stderr, "error: cannot open file %s'\n'", argv[1]);
            fclose(inputFile);
            exit(1);
        }
        if (strcmp(argv[1], argv[2]) == 0)
        {
            fprintf(stderr, "Input and output file must differ\n");
            fclose(inputFile);
            fclose(outputFile);
            exit(1);
        }

        while (fgets(str, 50, inputFile) != NULL)
        {
            fprintf(outputFile, "%s", str);
        }
        fclose(inputFile);
        fclose(outputFile);
    }

    return 0;
}