#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 1  // Set to 1 to turn on some debugging output, or 0 to turn off

/**
 * Parse the command line.
 *
 * YOU DON'T NEED TO MODIFY THIS!
 * (But you should study it to see how it works)
 *
 * Takes a string like "ls -la .." and breaks it down into an array of pointers
 * to strings like this:
 *
 *   args[0] ---> "ls"
 *   args[1] ---> "-la"
 *   args[2] ---> ".."
 *   args[3] ---> NULL (NULL is a pointer to address 0)
 *
 * @param str {char *} Pointer to the complete command line string.
 * @param args {char **} Pointer to an array of strings. This will hold the result.
 * @param args_count {int *} Pointer to an int that will hold the final args count.
 *
 * @returns A copy of args for convenience.
 */
char **parse_commandline(char *str, char **args, int *args_count)
{
    char *token;
    
    *args_count = 0;

    token = strtok(str, " \t\n\r"); 
    // takes entire string: delimeters are tab, new line, return
    // token is a pointer to the first token found in the string if token found
    // token is a null pointer if no tokens found

    while (token != NULL && *args_count < MAX_TOKENS - 1) { 
        // if null pointer, does not do while loop
        // if args_count is greater than or equal to MAX_TOKENS - 1, does not do while loop
        args[(*args_count)++] = token; 
        // the element in args at index args_count + 1 is now the token pointer

        token = strtok(NULL, " \t\n\r"); 
        // loops through the string token is pointing to (of tokens)
        // and if it finds a delimiter, replaces it with a null byte to terminate the current token
        // strtok then saves the following byte which will be the pointer to start searching for next token
    }

    args[*args_count] = NULL; // sets the element at args_count to null in args

    return args;
}

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS]; // char *args[100]

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Print a prompt
        printf("%s", PROMPT);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin);
        // reads from stdin and stores it in commandline to the maximum of 1024

        // Exit the shell on End-Of-File (CRTL-D)
        // CTRL-D is translated as end of standard input
        if (feof(stdin)) {
            break;
        }

        // Parse input into individual arguments
        parse_commandline(commandline, args, &args_count);

        if (args_count == 0) {
            // If the user entered no commands, do nothing
            continue;
        }

        // Exit the shell if args[0] is the built-in "exit" command
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        #if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]
        for (int i = 0; args[i] != NULL; i++) {
            printf("%d: '%s'\n", i, args[i]);
        }

        #endif
        
        /* Add your code for implementing the shell's logic here */
        
    }

    return 0;
}