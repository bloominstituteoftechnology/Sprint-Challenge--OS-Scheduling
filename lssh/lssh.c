#include <stdio.h>  // printf() function access
#include <stdlib.h> // exit() function access
#include <unistd.h> // system call functions like fork(), pipe() and I/O primitives
#include <string.h> // various functions for manipulating arrays of characters a.k.a. strings
// Defining macro variables
#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 0 // Set to 1 to turn on some debugging output, or 0 to turn off

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
    char *token; // pointer to a token string

    *args_count = 0; // initializing a counter pointer
    // strtok() is a function that takes a string and a delimiter (things that when present you want the string to split the thing before it and the thing after it). The function acts like split() in JS.
    token = strtok(str, " \t\n\r"); // returns a pointer to the first token found in the string. NULL pointer returned if there are no tokens left to retrieve.

    while (token != NULL && *args_count < MAX_TOKENS - 1) // while token is not NULL and argument count is less than max tokens - 1 (args array is not full)
    {
        args[(*args_count)++] = token; // increment the index of the array of strings args to place token

        token = strtok(NULL, " \t\n\r"); // advance to the next token
    }

    args[*args_count] = NULL;

    return args; // return the args array with the broken up command line string tokens
}

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS];

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1)
    {
        // Print a prompt
        printf("%s", PROMPT);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin);

        // Exit the shell on End-Of-File (CRTL-D)
        if (feof(stdin))
        {
            break;
        }

        // Parse input into individual arguments
        parse_commandline(commandline, args, &args_count);

        if (args_count == 0)
        {
            // If the user entered no commands, do nothing
            continue;
        }

        // Exit the shell if args[0] is the built-in "exit" command
        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        // Checking to see if args[0] is equal to cd (change directory)
        if (strcmp(args[0], "cd") == 0)
        {
            // grab the path the user specified to cd into and make sure the user supplied the path
            if (args_count != 2) // if they did not specify path they want to cd into
            {
                printf("usage: cd dirname\n"); // tell the user they need to provide a directory name after cd
                continue;
            }

            if (chdir(args[1]) < 0) // chdir() returns -1 if it was unsuccessful. The current directory remains unchanged.
            {
                // Provide an error message telling user shell failed to switch to provided directory
                fprintf(stderr, "Failed to switch directory to %s\n", args[1]);
            }

            continue; // Execute a continue statement to short-circuit the rest of the main loop
        }
        int bg = 0; // initialize a bg variable to signify state of background task (disabled as default or enabled)

        // Checking to see if the end of the args array contains an &
        if (strcmp(args[args_count - 1], "&") == 0)
        {
            // flip a boolean to indicate background task should be enabled
            bg = 1;
            // Strip the & off the args (by setting that pointer to NULL).
            args[args_count - 1] = NULL;
        }

        while (wait(NULL) > 0)
            ;

#if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]
        for (int i = 0; args[i] != NULL; i++)
        {
            printf("%d: '%s'\n", i, args[i]);
        }

#endif

        /* Add your code for implementing the shell's logic here */
        int rc = fork(); // Fork a child process to run the new command
        if (rc < 0)      // if fork failed print out a standard error message
        {
            fprintf(stderr, "Fork failed\n");
            continue; // Give user another prompt, don't exit out of shell completely here
        }
        else if (rc == 0)
        {
            // Exec the command in the child process (child process will no longer be a copy of parent process after execvp())
            execvp(args[0], args); // takes two parameters, a program name and uses the PATH environment variable to find the executable file, and the array of pointers terminated by a NULL pointer

            fprintf(stderr, "Exec failed\n"); // Error handling and exit. Tells us that child process failed to transform into the program we specified.
            exit(1);
        }
        else
        {
            //Parent process waits for child to complete if the bg boolean is false
            if (!bg)
            {
                int wc = waitpid(rc, NULL, 0);
            }
        }
    }

    return 0;
}