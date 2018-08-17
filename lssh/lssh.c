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

  while (1)
  {
    printf("%s", PROMPT);
    fflush(stdout);
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

    /* Add your code for implementing the shell's logic here */
  }

  return 0;
}