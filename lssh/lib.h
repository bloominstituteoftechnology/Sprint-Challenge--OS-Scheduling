#ifndef lib_h
#define lib_h

#include <stdio.h>
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

/**
 * Prints shell prompt
 *
 * @param str {char *} Pointer to the string to print.
 */
void print_prompt(char *str)
{
  printf("%s", str);
  fflush(stdout);
}

/**
 * Checks if requested command is exit
 *
 * @param args {char **} Pointer to parsed command line arguments
 *
 * @returns true(1) or false(0)
 */
int exit_shell(char **args)
{
  return strcmp(args[0], "exit") == 0;
}

/**
 * Checks if requested command is change directory
 *
 * @param args {char **} Pointer to parsed command line arguments
 * @param args_count {int} Number of command line arguments
 *
 * @returns true(1) or false(0)
 */
int change_directory(char **args, int args_count)
{
  return strcmp(args[0], "cd") == 0 && args_count == 2;
}

/**
 * Prints out the parsed command line for debuggin
 *
 * @param args {char **} Pointer to an array of strings.
 */
void debug_commandline_args(char **args)
{
  #if DEBUG
    for (int i = 0; args[i] != NULL; i++)
      printf("%d: '%s'\n", i, args[i]);
  #endif
}

#endif