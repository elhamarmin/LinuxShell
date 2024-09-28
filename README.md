# Custom Linux Shell

## Overview
This project implements a custom Linux shell that supports basic Unix commands as well as additional text processing functionalities. The shell is interactive, maintains command history, and allows piped execution of commands.

## Features

### Basic Commands:
- **cd**: Change the current working directory.
- **pwd**: Display the current working directory.
- **ls**: List directory contents.
- **exit**: Exit the shell.

### Custom Text Processing Commands:
- **A**: Prints the first word of each line from a text file.
- **B**: Prints the most frequently occurring word in the file.
- **C**: Removes all whitespace (spaces, tabs, newlines) from the text and displays the modified file content.
- **D**: Displays lines that are not commented (ignores lines starting with `#`).
- **F**: Counts and prints the number of lines in the file.
- **G**: Displays the first 10 lines of the file.

### Additional Functionalities:
- Multi-processing using `fork()`.
- Command history using `readline` library, saved to `history.txt`.
- Supports piped commands (e.g., `command1 | command2`).

### Optional Features (Bonus):
- Handles `Ctrl+C` without terminating the shell.
- Command history navigation using arrow keys (enabled by `readline`).
- Pipelining support for combining multiple commands.

## Usage

### Compilation:
To compile the shell, use the following command:

```bash
gcc -o custom_shell shell.c -lreadline
```
## Running the Shell
To run the shell:

```bash
./custom_shell
```
## Executing Commands:
You can run standard Linux commands like `ls`, `cd`, `pwd`, and the custom commands mentioned above. For text processing, specify the command and the file as arguments:

```bash
A filename.txt
B filename.txt
```
### Example for custom command A (prints the first word from each line):

```bash
A file.txt
```
### Piped Commands: 
Example of a piped command:

```bash
ls | grep "text"
```
### Example Session

```bash
/home/user/Desktop> cd /home/user/Documents
/home/user/Documents> ls
file1.txt file2.txt
/home/user/Documents> A file1.txt
first_word_of_each_line
```
## Technologies Used

- **Language:** C
- **Libraries:** `readline` for command history
- **System Calls:** `fork`, `execvp`, `getpid`, `wait`, `pipe`

## Notes

- The shell supports basic command execution and text processing.
- It utilizes `fork()` for process creation and `execvp()` for command execution.
- Custom commands such as `A`, `B`, `C`, `D`, `F`, and `G` are defined for text processing tasks.

## Contribution

Feel free to fork this project and submit pull requests for improvements or bug fixes!


