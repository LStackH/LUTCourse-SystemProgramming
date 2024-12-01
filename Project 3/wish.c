#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PATHS 100
#define MAX_ARGS 100

// Default shell path
char *paths[MAX_PATHS] = {NULL};

void interactiveMode();
void batchMode(const char *batchFile);
void executeCommand(char *line);
void executeParallelCommands(char *line);

void executeParallelCommands(char *line)
{
    char *command;
    pid_t pids[100]; // Array to store child process IDs
    int numProcesses = 0;

    // Split the input line into commands separated by '&'
    command = strtok(line, "&");
    while (command != NULL)
    {
        // Trim leading and trailing spaces
        while (*command == ' ')
        {
            command++;
        }
        char *end = command + strlen(command) - 1;
        while (end > command && *end == ' ')
        {
            *end-- = '\0';
        }

        // Check for built-in commands directly
        if (strncmp(command, "exit", 4) == 0)
        {
            executeCommand(command);
            return; // Exit after processing built-in `exit`
        }

        // Fork and execute each command in a child process
        pid_t pid = fork();
        if (pid == 0)
        {
            executeCommand(command);
            exit(0); // Child process exits after execution
        }
        else if (pid > 0)
        {
            pids[numProcesses++] = pid; // Store child PID
        }
        else
        {
            fprintf(stderr, "wish: fork failed\n");
            exit(1);
        }

        // Get the next command
        command = strtok(NULL, "&");
    }

    // Wait for all child processes to finish
    for (int i = 0; i < numProcesses; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
}

void interactiveMode()
{
    char *line = NULL;
    size_t len = 0;

    while (true)
    {
        printf("wish> ");
        fflush(stdout);

        if (getline(&line, &len, stdin) == -1)
        {
            free(line);
            exit(0);
        }

        executeParallelCommands(line);
    }
}

void batchMode(const char *batchFile)
{
    FILE *file = fopen(batchFile, "r");
    if (!file)
    {
        fprintf(stderr, "wish: cannot open file\n");
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, file) != -1)
    {
        executeParallelCommands(line);
    }

    free(line);
    fclose(file);
    exit(0);
}

void executeCommand(char *line)
{
    line[strcspn(line, "\n")] = '\0'; // Trim the trailing newline

    // Check for output redirection
    char *output_file = strstr(line, ">");
    if (output_file != NULL)
    {
        *output_file = '\0'; // Split the line at '>'
        output_file++;       // Move to the file name
        while (*output_file == ' ')
            output_file++; // Trim leading spaces

        if (*output_file == '\0')
        {
            fprintf(stderr, "wish: no output file specified\n");
            return;
        }

        char *parsed_file = strtok(output_file, " ");
        if (strtok(NULL, " ") != NULL)
        {
            fprintf(stderr, "wish: multiple redirection operators or files not allowed\n");
            return;
        }
        output_file = parsed_file;
    }

    // Tokenize the input line
    char *args[MAX_ARGS];
    int argc = 0;
    char *token = strtok(line, " ");
    while (token != NULL)
    {
        args[argc++] = token;
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;

    if (argc == 0)
    {
        return; // Empty command
    }

    // Built-in commands
    if (strcmp(args[0], "exit") == 0)
    {
        if (argc != 1)
        {
            fprintf(stderr, "wish: exit with too many arguments\n");
            return;
        }
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        if (argc != 2)
        {
            fprintf(stderr, "wish: cd expects exactly one argument\n");
            return;
        }
        if (chdir(args[1]) != 0)
        {
            perror("wish: cd");
        }
        return;
    }
    else if (strcmp(args[0], "path") == 0)
    {
        for (int i = 0; i < MAX_PATHS; i++)
        {
            if (paths[i] != NULL)
            {
                free(paths[i]); // Only free dynamically allocated memory
                paths[i] = NULL;
            }
        }
        for (int i = 1; i < argc; i++)
        {
            paths[i - 1] = strdup(args[i]);
        }
        paths[argc - 1] = NULL;
        return;
    }

    // External commands
    char full_path[1024];
    int found = 0;
    for (int i = 0; paths[i] != NULL; i++)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", paths[i], args[0]);
        if (access(full_path, X_OK) == 0)
        {
            found = 1;
            break;
        }
    }
    if (!found)
    {
        fprintf(stderr, "wish: command not found: %s\n", args[0]);
        return;
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        if (output_file != NULL)
        {
            freopen(output_file, "w", stdout);
            freopen(output_file, "w", stderr);
        }
        execv(full_path, args);
        fprintf(stderr, "wish: execv failed\n");
        exit(1);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        fprintf(stderr, "wish: fork failed\n");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
    paths[0] = strdup("/bin");

    if (argc == 1)
    {
        interactiveMode();
    }
    else if (argc == 2)
    {
        batchMode(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: wish [batch_file]\n");
        exit(1);
    }

    return 0;
}
