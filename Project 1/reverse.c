#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *data;
    struct Node *pNext;
} Node;

// add new nodes to the linked list
Node *addToBeginning(Node *head, char *str)
{
    Node *pNew = (Node *)malloc(sizeof(Node));
    if (!pNew)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    pNew->data = malloc(strlen(str) + 1);
    if (!pNew->data)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    strcpy(pNew->data, str);
    pNew->pNext = head; // Add at the beginning
    return pNew;
}

// free the whole linked list
void freeList(Node *head)
{
    // loop through linked list
    while (head)
    {
        Node *temp = head;
        head = head->pNext;
        free(temp->data);
        free(temp);
    }
}

// writing each nodes line to specified "outputFile"
void printOrWrite(Node *head, FILE *outputFile)
{
    Node *current = head;
    // loop through linked list
    while (current)
    {
        fprintf(outputFile, "%s", current->data);
        current = current->pNext;
    }
}

// main func, arg checking, specifying input and output files
int main(int argc, char *argv[])
{
    // too many args
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // initializing variables
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    Node *head = NULL;

    // 2 args, ./reverse.c "input"
    if (argc == 2)
    {
        inputFile = fopen(argv[1], "r");
        if (!inputFile)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        outputFile = stdout;
    }
    // 3 args, ./reverse.c "input" "output"
    else if (argc == 3)
    {
        // Check if input and output files are the same before opening the output file
        if (strcmp(argv[1], argv[2]) == 0)
        {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

        inputFile = fopen(argv[1], "r");
        if (!inputFile)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        outputFile = fopen(argv[2], "w");
        if (!outputFile)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            fclose(inputFile);
            exit(1);
        }
    }
    // no args, input and output to terminal
    else
    {
        inputFile = stdin;
        outputFile = stdout;
    }

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, inputFile) != -1)
    {
        // if we use keyword exit, we can exit manual input
        if (strcmp(line, "exit\n") == 0)
        {
            fprintf(stderr, "Exiting program...\n");
            break;
        }
        head = addToBeginning(head, line);
    }

    free(line);

    printOrWrite(head, outputFile);

    if (inputFile != stdin)
        fclose(inputFile);
    if (outputFile != stdout)
        fclose(outputFile);

    // free the linked list
    freeList(head);

    return 0;
}