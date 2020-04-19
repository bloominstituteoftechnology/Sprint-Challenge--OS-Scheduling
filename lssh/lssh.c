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
        // pid_t parentID;
        int result;
        pid_t childProcess = fork(); //fork a child process to run a new command

        // if (childProcess < 0)
        // {
        //     fprintf(stderr, "fork failed \n"); // call error if fork fails
        //     continue; // don't break out of the shell
        // }
        // else if (childProcess == 0)
        //     //in the childprocess's context
        // {
        //     if (strcmp(args[0], "cd") == 0 && args_count == 2)
        //     {
        //         result = chdir(args[1]);
        //         if (result == -1)
        //         {
        //             perror("chdir");
        //         }
        //     }

        //     execvp(args[0], args);
        //     continue;
        // }
        // else
        // {
        //     int waitProcessID = waitpid(childProcess, NULL, 0);
        // }

        if (childProcess < 0)
        {
            fprintf(stderr, "fork failed \n"); // call error if fork fails
            continue; // don't break out of the shell
        }

        if (childProcess == 0)
        {
            // in the child's process context
            execvp(args[0], args); //call execvp
            // childprocess failed to transform into the program that we specified
            fprintf(stderr, "Exec failed\n");
            exit(1); //exit from this context. The child will exit but the parent will continue

        } else {
            // back in the parent context. The parent doesn't finish execution until the child finishes
            wait(NULL); // will wait for all child processes to finish

        }

        return 0;
    }
}