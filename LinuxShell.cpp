// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<ctype.h>
#include<stdbool.h>
#include<signal.h>

#define MAXCOM 1000 
#define MAXLIST 100 


#define clear() printf("\033[H\033[J")

void updateHistory(char* str);
int hist = 0; 

void init_shell()
{
	clear();
	printf("\n\n\n\n******************"
		"************************");
	printf("\n\n\n\t****MY SHELL****");
	printf("\n\n\t-USE AT YOUR OWN RISK-");
	printf("\n\n\n\n*******************"
		"***********************");
	char* username = getenv("USER");
	printf("\n\n\nUSER is: @%s", username);
	printf("\n");
	sleep(1);
	clear();
}



int takeInput(char* str)
{
	char* buf;
	buf = readline("\n>>> ");
	if (strlen(buf) != 0) {
		updateHistory(buf);
		strcpy(str, buf);
		return 0;
	} else {
		return 1;
	}
}


void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\nDir: %s", cwd);
}


void execArgs(char** parsed)
{
	
	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr , "\nFailed forking child..");
		return;
	} else if (pid == 0) {
		if (execvp(parsed[0], parsed) < 0) {
			fprintf(stderr , "\nCould not execute command..");
		}
		exit(0);
	} else {
		wait(NULL);
		return;
	}
}

void execArgsPiped(char** parsed, char** parsedpipe)
{
    int pipefd[2];
    pid_t p1, p2;
    if (pipe(pipefd) < 0) {
        fprintf(stderr , "\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        fprintf(stderr , "\nCould not fork");
        return;
    }

    if (p1 == 0) {
        close(pipefd[0]);  
        dup2(pipefd[1], STDOUT_FILENO);  
        close(pipefd[1]); 
        if (execvp(parsed[0], parsed) < 0) {
            fprintf(stderr, "\nCould not execute command 1..");
            exit(1); 
        }
    } else {
 
        p2 = fork();
        if (p2 < 0) {
            fprintf(stderr , "\nCould not fork");
            return;
        }

        if (p2 == 0) {
            
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO);  
            close(pipefd[0]);  
            
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                fprintf(stderr , "\nCould not execute command 2..");
                exit(1); 
            }
        } else {
            close(pipefd[0]);
            close(pipefd[1]);

            waitpid(p1, NULL, 0);
            waitpid(p2, NULL, 0);
        }
    }
}


void openHelp()
{
	puts("\n***WELCOME TO MY SHELL HELP***"
		"\nCopyright @ Suprotik Dey"
		"\n-Use the shell at your own risk..."
		"\nList of Commands supported:"
		"\n>cd"
		"\n>ls"
		"\n>exit"
		"\n>history"
		"\n>all other general commands available in UNIX shell"
		"\n>pipe handling"
		"\n>improper space handling");

	return;
}

void updateHistory(char* str)
{

	if (!strcmp(str, "") || !strcmp(str, " ") || !strcmp(str, "\n")) return;
	FILE *f = fopen("history.txt", "a+");
	fprintf(f, "%s\n", str);
	fclose(f);
	hist++;
}

void printHistory()
{
	char *line = NULL;
    	size_t len = 0;
	FILE *f = fopen("history.txt", "a+");
	int i = 0;

	for (i = 0; i < hist; i++)
	{
		getline(&line, &len, f);
		printf("%d %s", i+1, line);
	}
}

void builtin_a(char** parsed)
{
    // Check if a file name was provided
    if (parsed[1] == NULL) {
        fprintf(stderr, "Error: No file name provided\n");
        return;
    }

    // Open the file
    FILE* fp = fopen(parsed[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
        return;
    }

    // Read the file one line at a time and print the first word
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char* first_word = strtok(line, " ");
        printf("%s\n", first_word);
    }

    // Close the file
    fclose(fp);
}

void builtin_b(char** parsed)
{
    // Check if a file name was provided
    if (parsed[1] == NULL) {
        fprintf(stderr, "Error: No file name provided\n");
        return;
    }

    // Open the file
    FILE* fp = fopen(parsed[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
        return;
    }

    // Read the file one line at a time and count the frequency of each word
        // Read the file one line at a time and count the frequency of each word
    char line[256];
    char* word;
    int word_count = 0;
    char most_frequent_word[256] = "";
    int most_frequent_word_count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Split the line into words
        word = strtok(line, " ");
        while (word != NULL) {
            // Check if the word is all alphabetic characters
            bool is_word = true;
            for (int i = 0; i < strlen(word); i++) {
                if (!isalpha(word[i])) {
                    is_word = false;
                    break;
                }
            }
            if (is_word) {
                word_count++;
                // Check if this is the most frequent word so far
                if (word_count > most_frequent_word_count) {
                    strcpy(most_frequent_word, word);
                    most_frequent_word_count = word_count;
                }
            }
            // Get the next word
            word = strtok(NULL, " ");
        }
        // Reset the word count for the next line
        word_count = 0;
    }

    // Print the most frequent word
    printf("%s\n", most_frequent_word);

    // Close the file
    fclose(fp);
}

void builtin_c(char** parsed)
{
    // Check if a file name was provided
    if (parsed[1] == NULL) {
        fprintf(stderr, "Error: No file name provided\n");
        return;
    }

    // Open the file
    FILE* fp = fopen(parsed[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
        return;
    }

    // Read the file one character at a time and print it if it is not a white space
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (!isspace(c)) {
            putchar(c);
        }
    }

    // Close the file
    fclose(fp);
}

void builtin_d(char** parsed)
{
    // Check if a file name was provided
    if (parsed[1] == NULL) {
        fprintf(stderr, "Error: No file name provided\n");
        return;
    }

    // Open the file
    FILE* fp = fopen(parsed[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
        return;
    }

    // Read the file one line at a time and print the lines that don't start with '#'
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Check if the line starts with '#'
        if (strlen(line) == 0 || line[0] != '#') {
            printf("%s", line);
        }
    }

    // Close the file
    fclose(fp);
}

void builtin_f(char** parsed)
{
    // Check if a file name was provided
    if (parsed[1] == NULL) {
        fprintf(stderr, "Error: No file name provided\n");
        return;
    }

    // Open the file
    FILE* fp = fopen(parsed[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
        return;
    }

    // Read the file one line at a time and count the number of lines
    char line[256];
    int line_count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Check if the line is not empty
        if (strlen(line) > 0) {
            line_count++;
        }
    }

    // Print the number of lines
    printf("%d\n", line_count);

    // Close the file
    fclose(fp);
}

void builtin_g(char** parsed)
{
    // Check if a file name was provided
    if (parsed[1] == NULL) {
        fprintf(stderr, "Error: No file name provided\n");
        return;
    }

    // Open the file
    FILE* fp = fopen(parsed[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
        return;
    }

    // Read the file one line at a time and print the first 10 lines
    char line[256];
    int line_count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Check if the line is not empty
        if (strlen(line) > 0) {
            line_count++;
        }
        if (line_count <= 10) {
            printf("%s", line);
        } else {
            break;
        }
    }

    // Close the file
    fclose(fp);
}

int ownCmdHandler(char** parsed)
{
	int NoOfOwnCmds = 10, i, switchOwnArg = 0;
	char* ListOfOwnCmds[NoOfOwnCmds];
	char* username;

	ListOfOwnCmds[0] = "exit";
	ListOfOwnCmds[1] = "cd";
	ListOfOwnCmds[2] = "help";
	ListOfOwnCmds[3] = "hello";
	ListOfOwnCmds[4] = "A";
	ListOfOwnCmds[5] = "B";
	ListOfOwnCmds[6] = "C";
	ListOfOwnCmds[7] = "D";
	ListOfOwnCmds[8] = "F";
	ListOfOwnCmds[9] = "G";
	
	for (i = 0; i < NoOfOwnCmds; i++) {
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
			switchOwnArg = i + 1;
			break;
		}
	}
	switch (switchOwnArg) {
	case 1:
		printf("\nGoodbye\n");
		exit(0);
	case 2:
		chdir(parsed[1]);
		return 1;
	case 3:
		openHelp();
		return 1;
	case 4:
		username = getenv("USER");
		printf("\nHello %s.\nMind that this is "
			"not a place to play around."
			"\nUse help to know more..\n",
			username);
		return 1;
	case 5: 
		builtin_a(parsed);
		return 1;
	case 6: 
		builtin_b(parsed);
		return 1;
	case 7: 
		builtin_c(parsed);
		return 1;
	case 8: 
		builtin_d(parsed);
		return 1;
	case 9: 
		builtin_f(parsed);
		return 1;
	case 10: 
		builtin_g(parsed);
		return 1;
	default:
		break;
	}

	return 0;
}


int parsePipe(char* str, char** strpiped)
{
	int i;
	for (i = 0; i < 2; i++) {
		strpiped[i] = strsep(&str, "|");
		if (strpiped[i] == NULL)
			break;
	}
	if (strpiped[1] == NULL)
		return 0; 
	else {
		return 1;
	}
}


void parseSpace(char* str, char** parsed)
{
	int i;
	for (i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, " ");
		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

int processString(char* str, char** parsed, char** parsedpipe)
{
	char* strpiped[2];
	int piped = 0;
	piped = parsePipe(str, strpiped);
	if (piped) {
		parseSpace(strpiped[0], parsed);
		parseSpace(strpiped[1], parsedpipe);
	} else {
		parseSpace(str, parsed);
	}
	if (ownCmdHandler(parsed))
		return 0;
	else
		return 1 + piped;
}



int main()
{
	FILE *f; 
	char inputString[MAXCOM], *parsedArgs[MAXLIST];
	char* parsedArgsPiped[MAXLIST];
	int execFlag = 0;
	init_shell();
	
	f = fopen("history.txt" , "w+");
	fclose(f);
	

	while (1) {
		printDir();
		if (takeInput(inputString))
			continue;
		execFlag = processString(inputString,
		parsedArgs, parsedArgsPiped);
		if (execFlag == 1)
			execArgs(parsedArgs);
		if (execFlag == 2)
			execArgsPiped(parsedArgs, parsedArgsPiped);
	}
	return 0;
}


