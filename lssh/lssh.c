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

    // strtok() breaks str into a series of tokens or break it into smaller sub-string.
    // \t\n\r = tab, new line, return
    // grab the first token
    token = strtok(str, " \t\n\r"); 
    // then do the loop grab token until NULL
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
        // fflush() used for output stream only
        // its purpose is to clear the output buffer and move the buffered data to console (stdout)
        // or disk (file output stream)
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        // fgets() reads a line from the specified stream and stores it into the string pointed to by str. 
        fgets(commandline, sizeof commandline, stdin);

        // Exit the shell on End-Of-File (CRTL-D)
        // feof () tests the end-of-file indicator for the given stream.
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

        // ========== Task 2: cd ========== //
        // compare args[0] == cd
        if (strcmp(args[0], "cd") == 0)
        {
            if (args_count < 2)
            {
                fprintf(stderr, "need directory - cd dirname\n");
                continue; 
            }
            // int chdir(const char *path)
            // chdir() changes the current dir of the calling process to the dir specified in path.
            // On success, 0 is returned. On error, -1 is returned
            if (chdir(args[1]) < 0)
            {
                fprintf(stderr, "No such file or directory\n");
                continue;
            }
            continue; 
        }

        /**
         * #if is a preprocessor command, which gets evaluated before the actual compilation step. 
         * The code inside that block doesn't appear in the compiled binary.
         * It's often used for temporarily removing segments of code 
         * with the intention of turning them back on later.
        */
        #if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]
        for (int i = 0; args[i] != NULL; i++) {
            printf("%d: '%s'\n", i, args[i]);
        }

        #endif   // close #if
        
        /* Add your code for implementing the shell's logic here */

        // ========== Task 1: lambda-shell$ ========== //

        // fork a child process to run the new command
        // printf("Initial Process (pid: %d)\n", (int) getpid());
        int rc = fork();

        if (rc < 0)
        {
            fprintf(stderr, "fork failed!\n");
            continue;   // continue the loop
        }
        if ( rc == 0) {
            // printf("Child Process (pid: %d)\n", (int) getpid());
            // exec the command in the child process
            execvp(args[0], args);
            fprintf(stderr,"execvp failed!\n");
            exit(1);    // as it's a child so dont need to keep alive.
        }
        else
        {
            // parent process waits for child to complete
            waitpid(rc, NULL, 0);   // can use wait(NULL) too
            // printf("Parent Process (pid: %d)\n", (int) getpid());
        }
    }

    return 0;
}