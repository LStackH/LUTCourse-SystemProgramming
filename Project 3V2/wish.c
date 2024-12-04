#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_TOKENS 100
#define MAX_PATHS 100
#define DELIMITERS " \t\r\n"

// Different error messages
#define DEFAULT_ERROR_MESSAGE "An error has occurred\n"
#define ERROR_FILE_OPEN "Error: Couldn't open file\n"
#define ERROR_WISH_MANYENTRY_ARGS "Error: Too many arguments, usage wish <file>\n"
#define ERROR_EXIT_ARGS "Error: exit takes no arguments\n"
#define ERROR_CD_ARGS "Error: cd requires exactly one argument\n"
#define ERROR_CD_FAIL "Error: cd failed\n"
#define ERROR_COMMAND_NOT_FOUND "Error: command not found\n"
#define ERROR_REDIRECTION_SYNTAX "Error: invalid redirection syntax\n"

char *paths[MAX_PATHS];
int num_paths = 0;

// Error message printing, with chosen error message
void print_error(const char *message)
{
    write(STDERR_FILENO, message, strlen(message));
}

// Parse input line into constituent pieces with given delimiters
char **parse_line(char *line, const char *delim, int *num_tokens)
{
    char **tokens = malloc(MAX_TOKENS * sizeof(char *));
    char *token = strtok(line, delim);
    int index = 0;

    while (token != NULL)
    {
        tokens[index++] = strdup(token);
        token = strtok(NULL, delim);
    }
    tokens[index] = NULL;
    *num_tokens = index;
    return tokens;
}

// Fins the executable in the current path
char *find_executable(char *command)
{
    for (int i = 0; i < num_paths; i++)
    {
        char *full_path = malloc(strlen(paths[i]) + strlen(command) + 2);
        strcpy(full_path, paths[i]);
        strcat(full_path, "/");
        strcat(full_path, command);

        if (access(full_path, X_OK) == 0)
        {
            // Executable found
            return full_path;
        }
        free(full_path);
    }
    // Didn't find executable
    return NULL;
}

// Execute a single command
void execute_command(char **args, int num_args)
{
    // Check for redirection
    int redirect = 0;
    char *output_file = NULL;
    for (int i = 0; i < num_args; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            if (i + 1 >= num_args || i + 2 != num_args)
            {
                print_error(ERROR_REDIRECTION_SYNTAX);
                return;
            }
            redirect = 1;
            output_file = args[i + 1];
            args[i] = NULL;
            break;
        }
    }

    // Built-in commands
    if (strcmp(args[0], "exit") == 0)
    {
        if (num_args != 1)
        {
            print_error(ERROR_EXIT_ARGS);
            return;
        }
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        if (num_args != 2)
        {
            print_error(ERROR_CD_ARGS);
            return;
        }
        if (chdir(args[1]) != 0)
        {
            print_error(ERROR_CD_FAIL);
        }
        return;
    }
    else if (strcmp(args[0], "path") == 0)
    {
        num_paths = 0;
        for (int i = 1; i < num_args; i++)
        {
            paths[num_paths++] = strdup(args[i]);
        }
        return;
    }

    // External commands
    char *exec_path = find_executable(args[0]);
    if (exec_path == NULL)
    {
        print_error(ERROR_COMMAND_NOT_FOUND);
        return;
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        if (redirect)
        {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            if (fd < 0)
            {
                print_error(DEFAULT_ERROR_MESSAGE);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }
        if (execv(exec_path, args) == -1)
        {
            print_error(DEFAULT_ERROR_MESSAGE);
            exit(1);
        }
    }
    else if (pid < 0)
    {
        // Fork failed
        print_error(DEFAULT_ERROR_MESSAGE);
    }
    else
    {
        // Parent process
        waitpid(pid, NULL, 0);
    }
    free(exec_path);
}

// Process line of input and parallel commands
void process_line(char *line)
{
    char *commands = strtok(line, "&");
    char *command_list[MAX_TOKENS];
    int num_commands = 0;

    while (commands != NULL)
    {
        command_list[num_commands++] = strdup(commands);
        commands = strtok(NULL, "&");
    }

    pid_t pids[num_commands];
    int num_pids = 0;

    for (int i = 0; i < num_commands; i++)
    {
        // Remove leading and trailing whitespace
        char *cmd = command_list[i];
        while (*cmd == ' ' || *cmd == '\t')
            cmd++;
        int len = strlen(cmd);
        while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '\t' || cmd[len - 1] == '\n'))
        {
            cmd[len - 1] = '\0';
            len--;
        }

        if (strlen(cmd) == 0)
            continue;

        int num_args = 0;
        char **args = parse_line(cmd, DELIMITERS, &num_args);
        if (args[0] == NULL)
        {
            free(args);
            continue;
        }

        // Check for built-in commands outside of fork
        if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "cd") == 0 || strcmp(args[0], "path") == 0)
        {
            execute_command(args, num_args);
            free(args);
            continue;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            execute_command(args, num_args);
            exit(0);
        }
        else if (pid < 0)
        {
            // Fork failed
            print_error(DEFAULT_ERROR_MESSAGE);
        }
        else
        {
            // Parent process
            pids[num_pids++] = pid;
        }
        free(args);
    }

    // Wait for all child processes
    for (int i = 0; i < num_pids; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
}

// Main func, entry point for wish
int main(int argc, char *argv[])
{
    FILE *input = stdin;
    char *line = NULL;
    size_t len = 0;

    // Initialize default path
    paths[num_paths++] = "/bin";

    // Batch mode
    if (argc == 2)
    {
        input = fopen(argv[1], "r");
        if (input == NULL)
        {
            print_error(ERROR_FILE_OPEN);
            exit(1);
        }
    }
    // Too many arguments
    else if (argc > 2)
    {
        print_error(ERROR_WISH_MANYENTRY_ARGS);
        exit(1);
    }

    // Shell loop
    while (1)
    {
        if (argc == 1)
        {
            // Interactive mode: print prompt
            printf("wish> ");
        }

        if (getline(&line, &len, input) == -1)
        {
            // EOF reached
            free(line);
            exit(0);
        }

        // Skip empty lines
        if (strcmp(line, "\n") == 0)
        {
            continue;
        }

        process_line(line);
    }

    free(line);
    return 0;
}
