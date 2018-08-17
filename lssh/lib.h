#ifndef lib_h
#define lib_h

#include <string.h>
#include "constants.h"

/**
 * Parse the command line.
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

  while (token != NULL && *args_count < MAX_TOKENS - 1)
  {
    args[(*args_count)++] = token;
    token = strtok(NULL, " \t\n\r");
  }

  args[*args_count] = NULL;
  return args;
}

#endif