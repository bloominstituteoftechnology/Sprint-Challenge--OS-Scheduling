#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 0  // Set to 1 to turn on some debugging output, or 0 to turn off

// My little baby functions. Aww, so cute.
char **parse_commandline(char *str, char **args, int *args_count);
void flagSpecialArgs(char** args, int args_count, int *background, char *redirection);
void doCD(char** args, int args_count);
void writeToFile(char* args[], char redirection[]);

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];

    // Special Command Flags
    int background;
    char redirection[256]; // Doubles as flag and holder for filename

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS];

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Reset Flags
        background = 0;
        redirection[0] = 0;

        // Clear out the zombies
        while (waitpid(-1, NULL, WNOHANG) > 0)
            ;

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
        
        // Look for special argument characters (i.e. '&' and '>')
        // Set flags accordingly
        flagSpecialArgs(args, args_count, &background, redirection);

        /* Add your code for implementing the shell's logic here */
        int rc = fork();

        if (rc < 0) {
            fprintf(stderr, "Fork unsuccessful. Perhaps try using a knife?\n");
            exit(1);
        }
        else if (rc == 0) {
            // If the command is `cd`, we'll take care of it instead of exec()
            if (strcmp(args[0], "cd") == 0) {
                doCD(args, args_count);
                continue;
            }
            // If redirection[0] isn't 0 (i.e. overwritten by file name), print to file
            if (redirection[0] != 0) {
                writeToFile(args, redirection);
                continue;
            }
            // The default execution of the program
            execvp(args[0], args);
        }
        else {
            if (background == 0) {
                waitpid(rc, NULL, 0);
            }
            # if DEBUG
            else {
                printf("Parent went to get some cigarretes at the store.\n");
            }
            #endif
        }
    }

    return 0;
}

void doCD(char** args, int args_count)
{
    if (args_count != 2) {
        fprintf(stderr, "usage: cd <directory>\n");
    }
    else {
        // Execution is in the if conditional
        // Error handling accomplished by checking its return value
        if (chdir(args[1]) == -1) {
            perror("chdir");
        }
    }
}

void writeToFile(char* args[], char redirection[])
{
    int fd = open(redirection, O_CREAT|O_RDWR, 0644);
    // STRETCH: Variable to save default stdout
    // Source: https://stackoverflow.com/a/11042581
    int saved_stdout = dup(1);
    // Now point stdout (aka '1') to fd
    dup2(fd, 1);
    // Do that thang
    execvp(args[0], args);
    // Then clean up the mess
    dup2(saved_stdout, 1); // puts stdout to back to what it was
    close(saved_stdout);
    close(fd);
}

void flagSpecialArgs(char* args[], int args_count, int *background, char redirection[])
{
    // Previous work for reference
    // int lastCharInd = args_count - 1;
    // printf("args_count: %d lastCharind: %d\n", args_count, lastCharInd);
    // printf("args[lastCharInd]: %s \n", args[lastCharInd]);
    // printf("args[lastCharInd][0]: %d\n", args[lastCharInd][0]);

    // if ((char) args[lastCharInd][0] == '&') {
    //     printf("FIRST CONDITION\n");
    //     *background = 1;
    //     args[lastCharInd] = NULL;
    //     printf("FIRST CONDITION OK\n");
    // }
    // else if (args_count > 2) {
    //     if ((char) args[lastCharInd - 1][0] == '>') {
    //         printf("SECOND CONDITION\n");
    //         strcpy(redirection, args[lastCharInd]);
    //         args[lastCharInd - 1] = NULL;
    //         args[lastCharInd] = NULL;
    //         printf("SECOND CONDITION OK\n");
    //     }
    // }

    int backgroundIndex = -1;
    int redirectIndex = -1; 

    for (int i = 0; i < args_count; i++) {
        if ((char) args[i][0] == '&') {
            backgroundIndex = i;
        }
        else if ((char) args[i][0] == '>') {
            redirectIndex = i;
        }
    }

    // Not a lot of syntax parsing or error handling unfortunately. Just the basics.
    // This approach allows '>' and '&' to be used together. My previous approach did not.
    if (backgroundIndex > -1) {
        *background = 1;
        args[backgroundIndex] = NULL;
    }
    if (redirectIndex > -1) {
        strcpy(redirection, args[redirectIndex + 1]);
        args[redirectIndex + 1] = NULL; 
        args[redirectIndex] = NULL;
    }
}

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
