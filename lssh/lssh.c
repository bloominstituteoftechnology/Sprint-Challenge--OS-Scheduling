#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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

#if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]

        for (int i = 0; args[i] != NULL; i++)
        {

            printf("%d: '%s'\n", i, args[i]);
        }

#endif

        /* Add your code for implementing the shell's logic here */

        //INSTRUCTOR SOLVE

        // NOTE: add the ability to cd

        if (strcmp(args[0], "cd") == 0)
        { // NOTE: arg 0 = cd string

            // NOTE: make sure that the user entered the expected number of args to the cd command

            if (args_count != 2)
            {

                fprintf(stderr, "usage: cd dirname\n");

                continue; //NOTE: loop continues & accept new command
            }

            // NOTE: use 'chdir' to change to the specified directory

            if (chdir(args[1]) < 0)
            {

                fprintf(stderr, "chdir failed\n"); //failed mssg

                continue;
            }

            // NOTE: add a 'continue' here for good measure

            continue;
        }

        // NOTE: use the fork-exec pattern

        pid_t child_pid = fork();

        // NOTE: error check on our fork call

        if (child_pid == -1)
        {

            fprintf(stderr, "fork failed\n");

            continue;
        }

        // NOTE: check for the child's context

        if (child_pid == 0)
        {

            // NOTE: call the exec command

            // NOTE: using whichever exec variant you prefer

            execvp(args[0], args);

            // NOTE: error check on exec

            fprintf(stderr, "exec failed\n");

            continue;
        }
        else
        {

            // NOTE: parent context 'wait's on the child

            waitpid(child_pid, NULL, 0);
        }
    }

    return 0;
}

// step 1-------------------------------------

//         int rc = fork();

//         if (rc < 0) {

//             fprintf(stderr, "Fork Failed\n");

//             exit(1);

//         } else if (rc == 0) {

//         // step 2-------------------------------------

//             if (strcmp(args[0], "cd") == 0) {

//                 if (args_count < 2)

//                 printf("Directory Needs To Be Specified\n");

//                 else {

//                     int change = chdir(args[1]);

//                     if(change < 0)

//                         perror("chdir");

//                     continue;

//                 }

//             } else

//             execvp(args[0], args);

//         }

//     }

//     return 0;

// }