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


        int rc = fork();
        if (rc < 0)
        {
            fprintf(stderr, "fork failure alert!\n");
            exit(1);
        }
        else if (rc == 0)
        {
            for (int i=0; i < args_count - 1; i++)
            {
                if (strcmp(args[i], ">") == 0)
                {
                    printf("is this working?\n");
                }
            }
            if (strcmp(args[0], "cd") == 0)  // if args[0] is cd
            {
            // make sure they entered at least 2 arguments
                if (args_count < 2)
                {
                    fprintf(stderr, "usage: cd [directory name]\n");
                    continue;
                }
            // run chdir() on 2nd argument to change directories
            // if chdir returns -1 print error message
                if (chdir(args[1]) < 0) 
                {
                    perror("chdir");
                    continue;
                }
                else
                {
                    chdir(args[1]);
                } 

            // continue
                continue;
            }
            else if (strcmp(args[args_count-1], "&") == 0) // if the last argument is &
// I originally had strcmp(args[2]), which I assure you is a bad idea
// since args is an array you can use the length (in this case args_count)
// -1 to get to the last item in the array
            {
                args[args_count-1] = NULL; // set & to NULL
                execvp(args[0], args); // run command as usual
                printf("%s", PROMPT); // create prompt - from lines 69-70 above
                fflush(stdout); // Force the prompt to print
// Note: I had trouble testing this, as every bash command I tried ran so
// quickly I got a prompt immediately anyway.

            }
            else 
            {
                execvp(args[0], args);
                fprintf(stderr, "oops! try again\n"); // the whole thing crashes if you don't do this
                continue;
            }
        }
        else
        {
            int wc = waitpid(rc, NULL, 0);
        }   

     
    }
    while (waitpid(-1, NULL, WNOHANG > 0)); // as per instructions in README
 
    return 0;
}
