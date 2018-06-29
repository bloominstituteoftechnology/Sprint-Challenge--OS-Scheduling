#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 1 // Set to 1 to turn on some debugging output, or 0 to turn off

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
 * @param args {char **} Pointer to an array of strings. This will hold the
 * result.
 * @param args_count {int *} Pointer to an int that will hold the final args
 * count.
 *
 * @returns A copy of args for convenience.
 */
char **parse_commandline(char *str, char **args, int *args_count) {
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

int launch(char **args) {
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// in child
		if (execvp(args[0], args) == -1) {
			perror("nope");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// bad fork
		perror("fork again");
	} else {
		// parent
    // wait(NULL);
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

char *func_str[] = {
  "cd",
  "help",
  "open",
  // "exit"
};

int term_num_funcs() {
  return sizeof(func_str) / sizeof(char *);
}

int term_cd (char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected arguement to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
  }
  return -1;
}

int term_open(char **args) {
  if(args[1] == NULL) {
    fprintf(stderr, "Shell: expected arguement to \"cd\"\n");
  } else {
    long int size = 0;
    FILE* file = fopen(args[1], "r");
    if(!file) {
      fputs("file error.\n", stderr);
      return 1;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    char* result = (char *) malloc(size);
    if(!result) {
      fprintf(stderr, "Memory error.\n");
      free(result);
      return 1;
    }

    if(fread(result, 1, size, file) != size) {
      fprintf(stderr, "read error\n");
      free(result);
      return 1;
    }

    fclose(file);
    if(!result) {
      fprintf(stderr, "fread result error\n");
      free(result);
      return 1;
    } else {
      printf("  %s\n", result);
      free(result);
    }
  }

  return 1;
}

int term_help(char **args) {
  int i;
  printf("Type program names and arguements, then hit enter.\n");
  printf("Available commands are: \n");
  for(i = 0; i < term_num_funcs(); i++) {
    printf("  %s\n", func_str[i]);
  }

  return 1;
}

// int term_exit(char **args) {
//   return 0;
// }

int (*function[]) (char **) = {
  &term_cd,
  &term_help,
  &term_open
  // &term_exit
};

int execute(char **args) {
  int i;

  if(args[0] == NULL) {
    return 1;
  }

  for(i = 0; i < term_num_funcs(); i++) {
    if(strcmp(args[0], func_str[i]) == 0) {
      return (*function[i])(args);
    }
  }

  return launch(args);
}

/**
 * Main
 */
int main(void) {
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
		for (int i = 0; args[i] != NULL; i++) {
			printf("%d: '%s'\n", i, args[i]);
		}

#endif

		/* Add your code for implementing the shell's logic here */

		int status;
    status = execute(args);
    if(!status) {
      return 0;
    }
	}
	return 0;
}