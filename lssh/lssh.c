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
        if (args_count == 2 && strcmp(args[0], "cd") == 0) {
                int cd = chdir(args[1]);
                continue;
                if (cd == -1){
                    perror("chdir");
                }            
            }

        char *redir_outfile = NULL;
        int fd[2];
        
        for (int i = 0; i < args_count; i++) {
            if (strcmp(args[i], ">") == 0){
                redir_outfile = args[i+1];
                args[i] = NULL;
            }
            else if (strcmp(args[i], "|") == 0) {
                if (pipe(fd) < 0) {
                    fprintf(stderr, "pipe failed\n");
                    exit(4);
                }
                else {
                  int pipefork = fork();

                  if (pipefork < 0) {
                    fprintf(stderr, "Fork failed");
                    exit(5);
                  }
                  else if (pipefork == 0) {
                    dup2(fd[0], 0);
                    close(fd[1]);
                    execlp(args[args_count-2], args[args_count-2], args[args_count-1], NULL);
                    
                    perror("exec child");
                    exit(6);
                  }
                  else {
                    dup2(fd[1], 1);
                    close(fd[0]);

                    execlp(args[0], args[0], args[1], NULL);
                    
                    perror("exec parent");
                    exit(7);
                  }
                }
            }
        }
        if (strcmp(args[(args_count-1)], "&") == 0) {
            args[args_count-1] = NULL;
            
            pid_t background = fork();
            if (background < 0){
                fprintf(stderr, "Background fork failed. \n");
                exit(2);
            }
            else if (background == 0){
                if (redir_outfile) {
                int fd = open(redir_outfile, O_CREAT|O_WRONLY, 0644);

                    if (fd < 0){
                        fprintf(stderr, "Failed to open file: %s", redir_outfile);
                        continue;
                        }
                    else {
                        dup2(fd, 1);
                        }
                    }
                if (execvp(args[0], args) == -1){
                    fprintf(stderr, "Background process failed to execute.");
                }
            }
        }   
        else {
            pid_t rc = fork();
            if (rc < 0) {
                fprintf(stderr, "Fork failed. \n");
                exit(1);
                }
            else if (rc == 0) {
                if (redir_outfile) {
                    int fd = open(redir_outfile, O_CREAT|O_WRONLY, 0644);
                    if (fd < 0){
                        fprintf(stderr, "Failed to open file: %s", redir_outfile);
                        continue;
                        }
                    else {
                        dup2(fd, 1);
                    }
                }
                if (execvp(args[0], args) == -1){
                    fprintf(stderr, "Child command failed to execute.");
                    }
                }
            else {
                wait(NULL);
            }
        }   
        
    }

    return 0;
}