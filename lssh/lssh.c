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

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
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
        // for (int i = 0; args[i] != NULL; i++) {
        //     printf("%d: '%s'\n", i, args[i]);
        // }

        #endif
        
        /* Add your code for implementing the shell's logic here */
        
        pid_t childPid;  // the child process that the execution will soon run inside of. 
        childPid = fork();

        if(childPid == 0)  // fork succeeded 
        {   
            if(strncmp (args[0] , "cd", 2) == 0){
                if(args_count == 2){
                    char cwd[1024];
                    getcwd(cwd, sizeof(cwd));
                    char* s1 = concat("/", args[1]);
                    char* s = concat(cwd, s1);
                    int xh = chdir(s);
                    if(xh == -1){
                        printf("error in ch");
                    }
                    else{
                        //printf("%d\n",xh);
                        continue;
                    }
                    printf("%s",s);
                    exit(0); 
                }
                else{
                    printf("brav you need 2 args for cd\n");
                }
            }
            else{
                char *cmd = args[0];
                char *argv[5];
                argv[0] = args[0];
                argv[1] = args[1];
                argv[2] = args[2];
                argv[3] = args[3];
                argv[4] = NULL;
                printf("\n");
                execvp(cmd, argv);   
                exit(0); 
            }
            
        }
        else if(childPid < 0)  // fork failed 
        {    
            printf("something went terribally wrong \n");
        }

        else  // Main (parent) process after fork succeeds 
        {    
            int returnStatus;    
            waitpid(childPid, &returnStatus, 0);  // Parent process waits here for child to terminate.

            if (returnStatus == 0)  // Verify child process terminated without error.  
            {
            printf("\n");    
            }

            if (returnStatus == 1)      
            {
            printf("The child process terminated with an error!.");    
            }
        }        
            
    }

    return 0;
}