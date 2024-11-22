CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi

Project 1: Warmup to C and Unix programming

Author: Lukas Honka

----------------------------------------------------

This project is a simple program, that takes an input and reverses it. It can display it in three different ways, depending on how the program is called. The three ways are:

prompt> ./reverse
prompt> ./reverse input.txt
prompt> ./reverse input.txt output.txt

When using the first one, the program will take input from the stdin, from the user, line by line as they press enter. To exit this, you can either press CTRL+D OR simply type "exit" and press enter, which prompts the program to stop and reverse the previous lines

Using the second way, you can specify from what file it takes the input from. It will read the files content from the start, line by line, and then output it,in reverse order, to the stdout, so the terminal.

Using the third way is similar to the second, but it only outputs the same content, into the specified output file.

----------------------------------------------------

The program uses a linked list, to dynamically allocate more space each time a new line is added. Since the idea is to reverse the input, the linked list is perfect since we keep track of the latest inputted line, and can from there follow the list back to the "original" beginning. 

In the main function, we initialize the input- and outputFile and assign them based on the given arguments. If none, it's stdin and stdout, then "input.txt" and stdout, then "input.txt" and "output.txt".

Function addToBeginning
