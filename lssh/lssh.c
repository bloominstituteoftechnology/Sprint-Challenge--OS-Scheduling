#include <stdio.h>  // install the necessary libraries
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>  // to print error message for chdir() system call     

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

        // implement "cd" command to change directories
        
        if (strcmp(args[0], "cd") == 0) // strcmp compares the string in the first argument args[0] with the string "cd"
        { 
            if (args_count != 2 ) //error handling; if arguments passed are not equal to 2
            {  
                fprintf(stderr, "Error. Please enter correct directory format: cd <directory_name>\n"); // print this error message
                continue;
            }

            // Run the chdir() system call on the second argument args[1] to change directories
            if (chdir(args[1]) < 0) // error handling; if second argument is less than 0 or -1  
            {
                perror("chdir");    // print this error message
                continue;
            }
            continue;
        }

       
        //Fork a child process to run the new command
        pid_t child_pid = fork(); // initialize child_pid and set it to the fork system call; pid_t is a data type for representing a process ID 

        // error handling
        if (child_pid < 0) {
            fprintf(stderr, "fork failed\n");   // if fork failed, print error message
            continue;    

        // Exec the command in the child process
        // Exec functions replaces the current process with the new process called by exec
        // This means that the child process can run a program different from what the parent is running
        if (child_pid == 0) {
            execvp(args[0], args); // runs the program for the child process taking in myargs
            fprintf(stderr, "Function failed to run.\n" );   // print in case of error  
            continue;
        
        // Parent process waits for child to complete
        } else {
            wait(child_pid, NULL, 0);   // wait function is called
        }        
    }

    return 0;
}