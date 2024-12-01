
# CT30A3370 Käyttöjärjestelmät ja Systeemiohjelmointi  

**Project 2: Implementing Unix Utilities**  
Author: Lukas Honka  

-----------------------------------------------------------------------------------------------------------------------------------------

## Overview  

This project implements four simple Unix-like utilities: `my-cat`, `my-grep`, `my-zip`, and `my-unzip`.  

### Utilities  

1. **my-cat**  
   Reads and outputs the content of one or more files to the terminal.  

2. **my-grep**  
   Searches for lines containing a specified search term in one or more files and outputs the matching lines.  

3. **my-zip**  
   Compresses the contents of files using a basic Run-Length Encoding (RLE) algorithm.  

4. **my-unzip**  
   Decompresses files that were compressed using `my-zip`.  

-----------------------------------------------------------------------------------------------------------------------------------------

## How It Works  

### my-cat  
This program reads the content of one or more files and outputs it line by line to the terminal. If no files are provided, the program does nothing and exits.  

Key Details:  
- Opens each specified file and reads its content using a buffer.  
- Prints the content directly to `stdout`.  
- Handles errors like trying to open a non-existent file.  

### my-grep  
This program searches for a specific term in one or more files and outputs all lines that contain the term.  

Key Details:  
- Takes the search term as the first argument and optionally a list of files.  
- If no files are provided, it reads from `stdin`.  
- Checks each line for the presence of the search term using `strstr`.  

### my-zip  
This program compresses the contents of one or more files using Run-Length Encoding (RLE).  

Key Details:  
- Reads each file character by character.  
- Groups consecutive identical characters together, storing the count and character in binary format.  
- Writes the compressed data to `stdout`.  

### my-unzip  
This program decompresses files that were compressed using `my-zip`.  

Key Details:  
- Reads the compressed binary format, which consists of an integer count and a character.  
- Reconstructs the original file by repeating each character `count` times.  
- Writes the decompressed data to `stdout`.  

-----------------------------------------------------------------------------------------------------------------------------------------

## Usage Examples  

**my-cat**  

When you run ./my-cat file1.txt file2.txt, the combined content of file1.txt and file2.txt is printed to the terminal.  

**my-grep**  

When you run ./my-grep searchterm file1.txt file2.txt, lines containing searchterm from file1.txt and file2.txt are printed to the terminal.  

If no files are provided, it reads from stdin.  

**my-zip**  

When you run ./my-zip file1.txt file2.txt > compressed_file, the compressed binary data of file1.txt and file2.txt is saved to compressed_file.  

**my-unzip**  

When you run ./my-unzip compressed_file, the decompressed content of compressed_file is printed to the terminal.  

-----------------------------------------------------------------------------------------------------------------------------------------