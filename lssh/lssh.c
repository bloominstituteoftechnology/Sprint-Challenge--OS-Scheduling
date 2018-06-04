#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 1

char **parse_commandline(char *str, char **args, int *args_count)
{
    char *token;

    *args_count = 0;

    token = strtok(str, " \t\n\r");

    while (token != NULL && *args_count < MAX_TOKENS - 1)
    {
        args[(*args_count)++] = token;

        token = strtok(NULL, " \t\n\r");
    }

    args[*args_count] = NULL;

    return args;
}

int main(void)
{
    char commandline[COMMANDLINE_BUFSIZE];

    char *args[MAX_TOKENS];

    int args_count;

    while (1)
    {

        printf("%s", PROMPT);
        fflush(stdout);

        fgets(commandline, sizeof commandline, stdin);

        if (feof(stdin))
        {
            break;
        }

        parse_commandline(commandline, args, &args_count);

        if (args_count == 0)
        {

            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

#if DEBUG

        for (int i = 0; args[i] != NULL; i++)
        {
            printf("%d: '%s'\n", i, args[i]);
        }

#endif

        int mystatus, wc;
        int fc = fork();

        if (fc < 0)
        {
            perror("You don goofed, be a better C programmer not a maggot!");
        }
        else if (fc == 0)
        {
            if (strcmp(args[0], "cd") == 0)
            {
                if (args_count < 2)
                    printf("Enter directory..\n");
                else
                {
                    int cd = chdir(args[1]);
                    if (cd < 0)
                        perror("You don goofed, be a better C programmer not a maggot!");
                    continue;
                }
            }
            else if (execvp(args[0], args) < 0)
                perror("You don goofed, be a better C programmer not a maggot!");
            exit(1);
        }
        else
        {
            do
            {
                wc = waitpid(fc, &mystatus, WUNTRACED);
            } while (!WIFEXITED(mystatus) && !WIFSIGNALED(status));
        }
    }

    return 0;
}