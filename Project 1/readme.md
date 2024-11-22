CT30A3370 Käyttöjärjestelmät ja Systeemiohjelmointi

Project 1: Warmup to C and Unix Programming

Author: Lukas Honka

-----------------------------------------------------------------------------------------------------------------------------------------

Overview:

This project is a simple C program called reverse that takes input and outputs it in reverse order. It can be used in three ways:

Terminal Input to Terminal Output:

* When you run ./reverse with no arguments, the program reads input line by line from the user and outputs the reversed lines back to the terminal. To stop entering input, press CTRL+D or type "exit" and press Enter.


File Input to Terminal Output:

* When you run ./reverse input.txt, the program reads lines from input.txt, reverses them, and outputs the reversed content to the terminal.


File Input to File Output:

* When you run ./reverse input.txt output.txt, the program reads lines from input.txt, reverses them, and writes the output to output.txt. NOTE: Both files cannot be the same.


-----------------------------------------------------------------------------------------------------------------------------------------

How It Works:

The program uses a linked list to store the input lines dynamically. Each new line is added to the beginning of the list, which means that the currently tracked beginning of the list, is the original's end. We can then follow the list back to the original start.


Functions
addToBeginning:

* Adds a new line to the start of the linked list.
* Allocates memory for both the new node and the string it stores.

freeList:

* Frees all memory used by the linked list.
* Goes through each node, freeing both the string and the node itself.

printOrWrite:

* Writes the reversed lines from the linked list to the specified output (terminal or file).

main:

* Handles argument validation and file setup.
* Reads input line by line, adding each line to the linked list.
* Outputs the reversed lines and ensures proper cleanup of resources like memory and files.

-----------------------------------------------------------------------------------------------------------------------------------------

Usage Examples
Interactive Mode:
When you type input directly into the program:

Hello
World

exit

Output:

World
Hello

File Input to Terminal Output:

When you provide an input file:

Input file (input.txt):

Line 1

Line 2

Line 3


Command: ./reverse input.txt

Output:

Line 3

Line 2

Line 1

File Input to File Output:

When you specify both input and output files:

Command: ./reverse input.txt output.txt

The reversed content is written to output.txt
