#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PROMPT "lambda-shell$ "

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

        // #if DEBUG

        //         // Some debugging output

        //         // Print out the parsed command line in args[]
        //         for (int i = 0; args[i] != NULL; i++)
        //         {
        //             printf("%d: '%s'\n", i, args[i]);
        //         }

        // #endif

        /* Add your code for implementing the shell's logic here */
        if (strcmp(args[0], "cd") == 0 && args_count != 2)
        {
            printf("Error, you need to provide argument for pwd\n");
            exit(1);
        };
        if (strcmp(args[0], "cd") == 0 && args_count == 2)
        {
            int rc = chdir(args[1]);
            if (rc == -1) {
                fprintf(strerror, "chdir failed\n");
                exit(1);
            }
            // argarray[0] =
            int size = 100;
            char string[size];

            // argarray[0] = args[1];
            // argarray[1] = NULL;
            getcwd(string, size);
            // chdir(args[1]);
            // argarray[2] = NULL;
            // argarray[1] = args[1];
            // args
            // printf("%s \n", string);
            continue;
            // execvp(argarray[0], argarray);
            // break;
        };
        int i;
        int rc = fork();
        if (rc < 0)
        {
            fprintf(stderr, "fork has failed\n");
            exit(1);
        }
        else if (rc == 0)
        {
            // printf("args[0] %s \n", args[0]);
            // printf("Child (pid: %d)\n", (int)getpid());
            // printf("args[0]: %s, args[1]: %s, args_count: %d \n", args[0], args[1], args_count);
            char *argarray[args_count];

            for (i = 0; args_count > i; i++)
            {
                argarray[i] = strdup(args[i]);
                // printf("argarray[i]: i %d \n", i);
                if (i == args_count - 1)
                {
                    // printf("end of i, i %d\n", i);
                    argarray[i + 1] = NULL;
                }
            };
            // argarray[0] = strdup(args[0]);
            // argarray[1] = strdup(args[1]);
            // argarray[2] = NULL;
            execvp(argarray[0], argarray);
            printf("If this message is read, there is something wrong with the child process");
        }
        else
        {
            waitpid(rc, NULL, 0);
            // printf("parent(pid: %d), child of parent %d\n", (int)getpid(), rc);
        }
    }

    return 0;
}