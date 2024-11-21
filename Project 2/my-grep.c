#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// search and print, from the given file, with the given search_term
void searchPrint(FILE *file, const char *search_term)
{
    char *line = NULL;
    size_t len = 0;

    // Read each line and check if the search term is in it
    while (getline(&line, &len, file) != -1)
    {
        // check if occurence of the substring *search_term*, in *line*
        if (strstr(line, search_term) != NULL)
        {
            printf("%s", line);
        }
    }

    free(line);
}

int main(int argc, char *argv[])
{
    // less than 2 args, exit
    if (argc < 2)
    {
        fprintf(stderr, "my-grep: searchterm [file ...]\n");
        exit(1);
    }

    // var, for what to look for
    const char *search_term = argv[1];

    // 2 args, no file, search from stdin
    if (argc == 2)
    {
        searchPrint(stdin, search_term);
    }
    // file given
    else
    {
        // go through each file
        for (int i = 2; i < argc; i++)
        {
            FILE *file = fopen(argv[i], "r");
            if (!file)
            {
                fprintf(stderr, "my-grep: cannot open file\n");
                exit(1);
            }

            searchPrint(file, search_term);

            fclose(file);
        }
    }

    return 0;
}
