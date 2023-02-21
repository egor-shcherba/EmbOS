#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

void*
cmd_echo(void *arg)
{
  char **args = (char**) arg + 1;

  while (*args != NULL)
    {
      printf("%s ", *args);
      args = args + 1;
    }

  printf("\n");

  return (void*) EXIT_SUCCESS;
}
