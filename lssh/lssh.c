#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROMPT "Lambda-shell Terminal\n\n"

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 1 // Set to 1 to turn on some debugging output, or 0 to turn off

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

    while (token != NULL && *args_count < MAX_TOKENS - 1)
    {
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
    char commandline[COMMANDLINE_BUFSIZE]; // string

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS]; // array of strings

    // How many command line args the user typed
    int args_count;

    // HOld current directory for pwd using buffer size for commandline
    char currDirect[COMMANDLINE_BUFSIZE];

    // Print a prompt
    printf("\n\n\n\033[31m%s\033[m", PROMPT);

    // Shell loops forever (until we tell it to exit)
    while (1)
    {
        getcwd(currDirect, 1024);
        printf("\033[31m<Dir>\033[m %s\n> ", currDirect);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin); // stdin == keyboard

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
            printf("Goodbye\n");
            break;
        }

        // comment out if you don't want to see the command line printed out word by word
        // #if DEBUG

        //         // Some debugging output

        //         // Print out the parsed command line in args[]
        //         for (int i = 0; args[i] != NULL; i++)
        //         {
        //             printf("%d: '%s'\n", i, args[i]);
        //         }

        // #endif

        /* Add your code for implementing the shell's logic here */

        /* Change Directories with 'cd' */
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "expected argument to \"cd\"\n");
                ;
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("cannot find");
                }
            }
        }

        /* 
        Fork first, then have the child process execute the args.
        Most commands are shell executable out of the box, so no need
        to write extra code other than execvp().
        */

        if (strcmp(args[0], "cd") != 0)
        {
            /* Execute Arbitrary Commands */
            int rc = fork();

            if (rc < 0)
            {
                fprintf(stderr, "fork failed\n"); // failed fork
            }
            else if (rc == 0)
            {
                // child process
                if (execvp(args[0], args) == -1)
                {
                    perror("cannot execute"); // throw error if execution fails
                }
                exit(EXIT_FAILURE);
            }
            else
            {
                // parent process
                wait();
            }
        }
    }

    return 0;
}