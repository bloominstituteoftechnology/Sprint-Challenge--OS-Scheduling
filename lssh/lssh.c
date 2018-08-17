#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PROMPT "lambda-shell$ "
#define errno (*__error())
#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 0  // Set to 1 to turn on some debugging output, or 0 to turn off

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
 * @returns A copy of args (the array of strings) for convenience.
 */
char **parse_commandline(char *str, char **args, int *args_count)
{
    char *token;
    
    *args_count = 0;

    token = strtok(str, " \t\n\r"); 
    //looks for spaces, tabs, newlines, or carriage returns
    while (token != NULL && *args_count < MAX_TOKENS - 1) { 
        // while token is not NULL and the args count is less than 99
        args[(*args_count)++] = token;
        //each args index increases the args count and returns it
        token = strtok(NULL, " \t\n\r"); 
        //token becomes NULL but doesn't break loop because of && condition
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
    char commandline[COMMANDLINE_BUFSIZE]; //1024

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS]; //100

    // How many command line args the user typed
    int args_count; // a number

    // Shell loops forever (until we tell it to exit) FOR-Ev-ER
    while (1) {
        // Print a prompt
        printf("%s", PROMPT); //"lambda-shell$ "
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin); 
            //stdin when user inputs...it's stdout when we fprint to it

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
        if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0) {
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

        //fork a child process to run the new command
        int rc = fork();
        //if fork fails
        if(rc < 0) {
            fprintf(stderr, "fork has failed\n");
            exit(1);
        } 
        //child process
        else if(rc == 0) {
            //if the string is cd
            if(strcmp(args[0], "cd") == 0) {
                //but comes with no args after
                if(args_count < 2) {
                    //throw an error!
                    fprintf(stdout, "usage: cd + <DIR>\n");
                    continue;
                } 
                else {
                    //instantiate chdir to be the directory you want to change to
                    int cd = chdir(args[1]);
                    if (cd == - 1) {
                        perror("chdir");
                        continue;
                    }
                continue;
                }
            }
            execvp(args[0], args);
        }
        else {
            waitpid(rc, NULL, 0);
        }
    }
    return 0;
}


