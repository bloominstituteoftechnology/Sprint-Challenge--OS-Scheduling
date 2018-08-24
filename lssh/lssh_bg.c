#include <stdio.h> //standard input/output
#include <stdlib.h>//standard library
#include <unistd.h>//defines miscellaneous symbolic constants and types,
// and declares miscellaneous functions
#include <string.h>//string functions/macros

#define PROMPT "lambda-shell-bg$ "

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
    // takes a string and a delimiter, and breaks down a string into "tokens"
    // This function returns a pointer to the first token found in the string.
    // A null pointer is returned if there are no tokens left to retrieve
    // Basically C equivelent of .split() from JS.
    // \t=tab,\n=newline,\r=carriage return
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

        if (strcmp(args[0], "cd") == 0)
        {
            // grab the path the user specified
            // make sure the user supplied the path
            if (args_count != 2)
            {
                printf("usage: cd <directory name>\n");
                continue;
            }

            if (chdir(args[1]) < 0)
            {
                fprintf(stderr, "Failed to switch directory to %s\n", args[1]);
                continue;
            }

            continue;
        }
        //bg=background, a boolean value, initially set to false
        int bg = 0;
        // check for the & at the end of the args array
        if (strcmp(args[args_count-1], "&") == 0)
        {
            // flip boolean value to true to indicate background
            // task should be enabled.
            bg = 1;
            // drop the &
            args[args_count-1] = NULL;
        }

        // this empty loop will just call wait() repeatedly until there are
        // no more zombie processes to reap, then the program continues on
        while(wait(NULL) > 0)
            ; 

        #if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]
        for (int i = 0; args[i] != NULL; i++) {
            printf("%d: '%s'\n", i, args[i]);
        }

        #endif
        
        /* Add your code for implementing the shell's logic here */
        pid_t child_pid = fork();
        //error check
        if (child_pid < 0)
        {
            fprintf(stderr, "Fork Failed\n");
            continue;
        }
        //more checking
        if (child_pid == 0)
        {
            //in child process context
            // execvp is used because we can pass arrays as arguements
            execvp(args[0], args);
            //child process failed to transform into the program we specified.
            fprintf(stderr, "Exec Failed\n");
            exit(1);
        }
        else
        {
            // parent context
            // if the background is false, we wait.
            if (!bg)
            {
                wait(NULL);
            }
            else
            {
                // not necessary, but more explicit
                continue;
            }
        }
        
    }

    return 0;
}
