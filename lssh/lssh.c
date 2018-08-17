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

    // Splits str[] according to given delimiters.
    // and returns next token. It needs to be called
    // in a loop to get all tokens. It returns NULL
    // when there are no more tokens.
    // char * strtok(char str[], const char *delims);

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
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];// 1024

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS];//100

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Print a prompt
        printf("%s", PROMPT);
        // fflush() is typically used for output stream only.
        // Its purpose is to clear (or flush) the output buffer and move the buffered 
        // data to console (in case of stdout) 

        //fflush(FILE *ostream);
        fflush(stdout); // Force the line above to print
        
        // It reads a line from the specified stream and stores it into the string pointed to by str. 
        // It stops when either (n-1) characters are read, the newline character is read, 
        // or the end-of-file is reached, whichever comes first.

        //char *fgets(char *str, int n, FILE *stream)
        fgets(commandline, sizeof commandline, stdin);// Read input from keyboard
        



        // C provides feof() which returns non-zero value only 
        // if end of file has reached, otherwise it returns 0.

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

        int rc = fork();
    
        if (rc < 0) {
            // perror: It displays the string you pass to it, 
            // followed by a colon, a space, and then the textual representation of the current errno value.
            perror( "Fork failed\n");
            exit(1);
        }
        else if (rc == 0) {
            /*
            check to see if the user entered `cd` in `args[0]` _before_ running the command. 
            And if they did, you should short-circuit the rest of the main loop with a `continue` 
            statement.
            If the user entered `cd` as the first argument:
            1. Check to make sure they've entered 2 total arguments
            2. Run the system call `chdir()` on the second argument to change directories
            3. Error check the result of `chdir()`. If it returns `-1`, meaning an error
            occurred, you can print out an error message with:
            ```
            perror("chdir"); // #include <errno.h> to use this
            ```
            4. Execute a `continue` statement to short-circuit the rest of the main loop.
            Note that `.` and `..` are actual directories. You don't need to write any special case code
             to handle them.
            */
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
            /*
            The first argument is the file you wish to execute, 
            and the second argument is an array of null-terminated strings that represent 
            the appropriate arguments to the file as specified in the man page.
            
            For example:
            char *cmd = "ls";
            char *argv[3];
            argv[0] = "ls";
            argv[1] = "-la";
            argv[2] = NULL;
            execvp(cmd, argv); //This will run "ls -la" as if it were a command
            */
            perror("execvp");
            exit(2);
        
        } else {
            waitpid(rc, NULL, 0);
        }
    }

    return 0;
}