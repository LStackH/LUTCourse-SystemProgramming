
# CT30A3370 Käyttöjärjestelmät ja Systeemiohjelmointi  

**Project 3: Implementing a Unix Shell**  
Author: Lukas Honka  

-----------------------------------------------------------------------------------------------------------------------------------------  

## Overview  

This project implements a basic Unix shell named `wish`. The shell processes user input, executes commands, and supports common shell features like built-in commands, redirection, and parallel command execution.  

### Key Features  

1. **Interactive and Batch Modes**  
   - Interactive mode accepts commands directly from the user.  
   - Batch mode reads commands from a file and executes them sequentially.  

2. **Built-in Commands**  
   - `exit`: Exits the shell.  
   - `cd`: Changes the current working directory.  
   - `path`: Modifies the search path for executable files.  

3. **Command Execution**  
   - Runs external programs using the specified search path. Default path `/bin`

4. **Redirection**  
   - Redirects the output of commands to a specified file using the `>` symbol.  

5. **Parallel Execution**  
   - Executes multiple commands simultaneously, separated by `&`.  

-----------------------------------------------------------------------------------------------------------------------------------------  

## How It Works  

### Interactive and Batch Modes  
- **Interactive Mode**: Displays the prompt `wish>` and processes commands entered by the user.
- **Batch Mode**: Reads commands from a specified file and executes them without displaying the prompt.

### Built-in Commands  
1. **`exit`**  
   - Exits the shell when invoked without arguments.  
   - Returns an error if additional arguments are provided.  

2. **`cd`**  
   - Changes the current working directory to the specified path.  
   - Returns an error if no arguments or more than one argument is provided, or if the directory does not exist.  

3. **`path`**  
   - Sets the shell's search path for executable programs.  
   - Clears the path if invoked without arguments, allowing only built-in commands to work.  

### External Commands  
- Searches for executables in the directories specified by the `path` command.  
- If an executable is found, it is executed with the provided arguments.  
- Displays an error if the command is not found or cannot be executed.  

### Redirection  
- Supports output redirection using the `>` symbol.  
- Writes both `stdout` and `stderr` of the command to the specified file.  
- Returns an error for invalid redirection syntax or multiple redirection operators.  

### Parallel Execution  
- Executes multiple commands concurrently using the `&` operator.  
- Waits for all parallel commands to complete before returning to the prompt or processing the next batch command.  

-----------------------------------------------------------------------------------------------------------------------------------------  

## Usage Examples  

**Interactive Mode**  

- Start the shell interactively with `./wish` and run commands like `ls`, `cd`, or `exit`.  

**Batch Mode**  

- Run a batch file that contains a sequence of commands, with command `./wish <file>`. The batch file must end with an empty line.

**Built-in Commands**  

- Use `exit` to terminate the shell.  
- Use `cd` to change directories.  
- Use `path` clear the executable search path. Use `path /usr/bin` for example, to set path to `/usr/bin`

**Redirection**  

- Use `>` to redirect the output of a command to a file. Example: `ls > output.txt`, will print current directory to output.txt

**Parallel Execution**  

- Use `&` to execute multiple commands concurrently. Example: `sleep 5& echo "running in parallel`, will run both in parallel, and wait until both have been completed.

-----------------------------------------------------------------------------------------------------------------------------------------  

## Error Handling  

- Displays error messages for invalid input, syntax, or execution issues.  
- Continues processing other commands after encountering an error.  
- Exits gracefully on `EOF` or batch file errors.  

-----------------------------------------------------------------------------------------------------------------------------------------  

## Notes  

- The shell is designed to handle varying amounts of whitespace and arbitrary line lengths.  
- The default search path includes `/bin`.  
- Built-in commands are executed directly, bypassing the search path.  
- External commands rely on the current `path` for execution. If path has been cleared, commands like `ls` and `clear` wont work.

-----------------------------------------------------------------------------------------------------------------------------------------  
