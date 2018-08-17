#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

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
    // takes entire string: delimeters are tab, new line, return
    // token is a pointer to the first token found in the string if token found
    // token is a null pointer if no tokens found

    while (token != NULL && *args_count < MAX_TOKENS - 1) { 
        // if null pointer, does not do while loop
        // stop while loop if args_count gets larger than or equal to MAX_TOKENS - 1
        args[(*args_count)++] = token; 
        // the element in args at index args_count is now the token pointer
        // increment args_count
        // args_count started at 0

        token = strtok(NULL, " \t\n\r"); 
        // loops through the string token is pointing to (of tokens)
        // and if it finds a delimiter, replaces it with a null byte to terminate the current token
        // strtok then saves the following byte which will be the pointer to start searching for next token
    }

    args[*args_count] = NULL; // sets the element at args_count to null in args

    return args;
}

static void sigchld_hdl (int sig)
{
    (void)sig;
    int saved_errno = errno; 
    // waitpid() might overright errno, so this saves and restores it
    while (waitpid(-1, NULL, WNOHANG) > 0);
    // wait for any child process
    // do not store information
    // WNOHANG specifies that it should return immediately if there are 
    // no child processes exited
    errno = saved_errno;
}

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS]; // char *args[100]

    // How many command line args the user typed
    int args_count;
    struct sigaction bg;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        bg.sa_handler = sigchld_hdl; // action associated with SIGCHLD
        sigemptyset(&bg.sa_mask); // this blocks other terminal-generated signals while this handler runs
        bg.sa_flags = SA_RESTART; // restarts the signal handler if it is interrupted
        if (sigaction(SIGCHLD, &bg, NULL) == -1) {
            // sigaction system call changes the action taken by process 
            // on receipt of specific signal (this case SIGCHLD)
            perror("sigaction");
            exit(3);
        }
        // Print a prompt
        printf("%s", PROMPT);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin);
        // reads from stdin and stores it in commandline to the maximum of 1024

        // Exit the shell on End-Of-File (CRTL-D)
        // CTRL-D is translated as end of standard input
        if (feof(stdin)) {
            printf("\n");
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
        printf("Number of args: %d\n", args_count);

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
        for (int i = 0; i < args_count; i++) {
            if (strcmp(args[i], ">") == 0){
                redir_outfile = args[i+1];
                args[i] = NULL;
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