#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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

    while (token != NULL && *args_count < MAX_TOKENS - 1) {
        args[(*args_count)++] = token;

        token = strtok(NULL, " \t\n\r");
    }

    args[*args_count] = NULL;

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
    char *args[MAX_TOKENS];

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Print a prompt
        printf("%s", PROMPT);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin);

        // Exit the shell on End-Of-File (CRTL-D)
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

        // Enables user to change directories with the "cd" command.
        // The strcmp() function compares the string from the first argument and the string from the second argument.
        if (strcmp(args[0], "cd") == 0)
        {
            if (args_count != 2)
            {
                fprintf(stderr, "Error. Please provide name of directory and follow correct format: cd <name_of_directory>\n");
                continue; // The 'continue' statement continues the loop.
            }
            // The chdir() function causes the directory named by the pathname pointed to by the path argument to become the current working directory.
            if (chdir(args[1]) < 0)
            {
                fprintf(stderr, "Error. No such directory.\n");
                continue;
            }
            continue;
        }

        // Enables user to execute arbitrary commands.
        // pid_t is a data type which is capable of representing a process ID.
        pid_t child_pid = fork();
        if (child_pid == -1)
        {
            fprintf(stderr, "Error. Failed to fork.\n");
            continue;
        }

        // The exec() function is a C library function that allows the child process to run a program different from that of the parent process.
        // The execvp() function is a varient of exec().
        // The exec family of functions replaces the current running process with a new process.
        // Using this command, the created child process does not have to run the same program as the parent process does. 
        // https://www.geeksforgeeks.org/exec-family-of-functions-in-c/
        if (child_pid == 0)
        {
            // Calls the exec function.
            execvp(args[0], args);
            fprintf(stderr, "Error. Exec function failed.\n");
            continue;
        } else {
            // Parent process waits on the child process.
            waitpid(child_pid, NULL, 0);
        }
    }

    return 0;
}