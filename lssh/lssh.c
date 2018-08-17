#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 1  // Set to 1 to turn on some debugging output, or 0 to turn off



char **parse_commandline(char *str, char **args, int *args_count)
{
    char *token;
    
    *args_count = 0;

    token = strtok(str, " \t\n\r");
    // \t = tab
    // \r = carriage  return
    // \n = newline

    while (token != NULL && *args_count < MAX_TOKENS - 1) {
        args[(*args_count)++] = token;

        token = strtok(NULL, " \t\n\r");
    }

    args[*args_count] = NULL;

    return args;// [arg1, arg2, arg3]
}

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user inputs
    char commandline[COMMANDLINE_BUFSIZE];// 1024

    //the parsed version of the command line
    char *args[MAX_TOKENS];//100

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Print a prompt
        printf("%s", PROMPT);

        //fflush(FILE *ostream);
        fflush(stdout); // Force the line above to print
        

        //char *fgets(char *str, int n, FILE *stream)
        fgets(commandline, sizeof commandline, stdin);// Read input from keyboard


        // Exiting from the shell 
        if (feof(stdin)) {
            break;
        }

        // Parsing input into individual arguments
        parse_commandline(commandline, args, &args_count);

        // If the user entered no commands, do nothing
        if (args_count == 0) {
            continue;
        }

        // Exit the shell 
        if (strcmp(args[0], "exit") == 0) {
            break;
        }


        #if DEBUG

        // Print out the parsed command line 
        for (int i = 0; args[i] != NULL; i++) {
            printf("%d: '%s'\n", i, args[i]);
        }

        #endif
        
        /* Add your code for implementing the shell's logic here */

        int rc = fork();
    
        if (rc < 0) {
            // perror: It displays the string you pass to it, 
            // followed by a colon, a space, and then the textual representation of the current errno value.
            perror( "Fork failed\n");
            exit(1);
        }
        else if (rc == 0) {

            if(strcmp(args[0], "cd") == 0) //if the user entered `cd` in `args[0]`
            {
                if(args_count == 2)
                {
                    if(chdir(args[1]) == -1)// Otherwise, -1 shall be returned, the current working directory shall remain unchanged, and errno shall be set to indicate the error
                    {
                        perror("chdir");
                    }
                }
                continue;
            }
           
            execvp(args[0], args);
       
            perror("execvp");
            exit(2);
        
        } else {
            waitpid(rc, NULL, 0);
        }
    }

    return 0;
}