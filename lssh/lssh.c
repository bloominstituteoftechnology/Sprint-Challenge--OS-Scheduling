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

    if (strcmp(args[0], "exit") == 0)
      break;
    if (args_count == 0)
      continue;

    #if DEBUG
      for (int i = 0; args[i] != NULL; i++)
        printf("%d: '%s'\n", i, args[i]);
    #endif

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