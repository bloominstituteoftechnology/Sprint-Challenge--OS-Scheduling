#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h> // header file for bool
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

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

static void sigchld_hdl (int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
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

    bool backgroundTask = 0;

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

        // Implement the ability to change directories with cd
        if (strcmp(args[0], "cd") == 0) {
            // check to ensure two arguments have been entered 
            if (args_count != 2)
            {
                printf("Usage: cd path \n");
            }
            else
            {
                int result = chdir(args[1]);
                if (result == -1)
                {
                    perror("chdir");
                }
            }
            continue;
        }

        // Background Tasks
        if (strcmp(args[args_count - 1], "&") == 0)
        {
            args[args_count - 1] = NULL;
            backgroundTask = 1;
        }

        // File Redirection
        for (int i = 0; i <args_count; i++)
        {
            if (strcmp(args[i], ">") == 0)
            {
            
            }
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
        { // fork failed; exit
            fprintf(stderr, "fork failed \n"); 
            exit(1);
        }
        else if (rc == 0) 
        { // child process satisfies this branch
            if (backgroundTask)
            {
                execvp(args[0], &args[0]);
            }
            else 
            {
                execvp(args[0], &args[0]);
            }
        }
        else
        { // adult process
            if (backgroundTask)
            {
                struct sigaction act;
                memset(&act, 0, sizeof(act));
                act.sa_handler = sigchld_hdl;
                if(sigaction(SIGCHLD, &act, 0)) 
                {
                    perror("sigaction \n");
                    return 1;
                }
            }
            else{
                waitpid(rc, NULL, 0);
            }
        }
    }
    

    return 0;
}