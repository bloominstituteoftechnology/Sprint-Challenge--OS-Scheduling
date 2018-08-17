#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
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
  int wait_for_child;
  int redirect_to_file;

  while (1)
  {
    wait_for_child = 1;
    redirect_to_file = 0;
    print_prompt(PROMPT);
    fgets(commandline, sizeof commandline, stdin);

    if (feof(stdin))
      break;

    parse_commandline(commandline, args, &args_count);
    debug_commandline_args(args);

    if (args_count == 0)
      continue;
    if (exit_shell(args))
      break;
    if (change_directory(args, args_count))
    {
      if (chdir(args[1]) == -1)
        print_prompt(CD_ERROR);
      continue;
    }
    if (background_task(args, args_count)){
      wait_for_child = 0;
      args[args_count - 1] = NULL;
    }
    if (file_redirect(args, args_count))
      redirect_to_file = 1;

    child = fork();
    if (child == 0)
    {
      if (redirect_to_file){
        char *filename = args[args_count - 1];
        args[args_count - 2] = NULL;
        args[args_count - 1] = NULL;
        int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        dup2(fd, 1);
      }
      if (execvp(args[0], args) == -1)
        print_prompt(COMMAND_NOT_FOUND);
      exit(0);
    } else {
      if (wait_for_child)
        waitpid(child, NULL, 0);
    }

    while (waitpid(-1, NULL, WNOHANG) > 0)
      ;
  }

  return 0;
}