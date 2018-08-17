#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "lib.h"

/**
 * Main
 */
int main(void)
{
  char commandline[COMMANDLINE_BUFSIZE];
  char *args[MAX_TOKENS];
  int args_count;
  int child;

  while (1)
  {
    print_prompt(PROMPT);
    fgets(commandline, sizeof commandline, stdin);

    if (feof(stdin))
      break;

    parse_commandline(commandline, args, &args_count);
    debug_commandline_args(args);

    if (args_count == 0)
      continue;
    if (strcmp(args[0], "exit") == 0)
      break;
    if (strcmp(args[0], "cd") == 0 && args_count == 2)
    {
      if (chdir(args[1]) == -1)
        print_prompt(CD_ERROR);
      continue;
    }

    child = fork();
    if (child == 0)
    {
      if (execvp(args[0], args) == -1)
        print_prompt(COMMAND_NOT_FOUND);
      exit(0);
    } else {
      waitpid(child, NULL, 0);
    }
  }

  return 0;
}