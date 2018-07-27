#include <stdio.h>
#include <string.h>

int main()
{
  char str[] = "-hello-word-0-";

  // Returns first token
  char *token = strtok(str, "-");

  // Keep printing tokens while one of the
  // delimiters present in str[].
  while (token != NULL)
  {
    printf("%s\n", token);
    token = strtok(NULL, "-");
  }

  return 0;
}