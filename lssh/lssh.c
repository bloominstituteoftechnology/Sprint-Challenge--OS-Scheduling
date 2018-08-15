#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>


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

void sigchld_handler(int s) {
  (void)s; // quiet unused variable warning

  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  // Wait for all children that have died, discard the exit status
  while(waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}

void start_reaper(void)
{
  struct sigaction sa;

  sa.sa_handler = sigchld_handler; // Reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART; // Restart signal handler if interrupted
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }
}

/**
 * Main
 */
int main(void)
{
    start_reaper();
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS];

    // How many command line args the user typed
    int args_count;
    int parent_pid = getpid();
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
            printf("See you next time :)\n");
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
        if(args_count == 0){
            printf("Please provide input\n");
            continue;
        }
        if(args_count == 1) 
        {   
            int child_pid = fork();
            wait(0);
            if(parent_pid != getpid()){
                execlp(args[0], args[0], (char *)0);
                exit(1);
                continue;
            }
            continue;
        }
        if(args_count == 2){
            int child_pid = fork();
            if(strcmp("&", args[1]) != 0){
                wait(0);
            }else {
                args[1] = NULL;
            }
            if(getpid() != parent_pid){
                if(strcmp(args[0], "cd") == 0){
                    int dirChange = chdir(args[1]);
                    if(dirChange == -1){
                        perror("chdir");
                    }
                    continue;
                    exit(0);
                }else{
                    execlp(args[0], args[0], args[1], (char*)0);
                    exit(1);
                    continue;
                }
            }else{
                continue;
            }
        }
        if(args_count == 3){
            int child_pid = fork();
            if(strcmp("&", args[2]) != 0){
                wait(0);
            }
            if(getpid() != parent_pid){
                execlp(args[0], args[0], args[1], args[2], (char*)0);
                exit(1);
                continue;
            }
        }

        if(args_count == 4){
            int child_pid = fork();
            if(strcmp("&", args[3]) != 0){
                wait(0);
            }
            if(getpid() != parent_pid){
                execlp(args[0], args[0], args[1], args[2], args[3], (char*)0);
                exit(1);
                continue;
            }
        }
        if(args_count == 5){
            int child_pid = fork();
            if(strcmp("&", args[4]) != 0){
                wait(0);
            }
            if(getpid() != parent_pid){
                execlp(args[0], args[0], args[1], args[2], args[3], args[4], (char*)0);
                exit(1);
                continue;
            }
        }
    }     
    return 0;
}