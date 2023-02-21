#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static char *help_msg =
            "Usage : sleep NUMBER\n"
            "Pause for NUMBER seconds\n"
            "   --help    display this help and exit\n"
            "   --version output version information and exit\n";

static char *version = "sleep v1.0 for EmbOS\n";

void*
cmd_sleep(void *arg)
{
  char **args = (char**) arg;
  unsigned n = 0;

  if (args[1] == NULL)
    {
      printf("sleep :: mising operand\n");
      printf("try sleep --help for more information\n");
      return (void*) EXIT_FAILURE;
    }

  if (strcmp(args[1], "--help") == 0)
    printf("%s", help_msg);
  else if (strcmp(args[1], "--version") == 0)
    printf("%s", version);
  else
    {
      n = atoi(args[1]);
      sleep(n);
    }

  return EXIT_SUCCESS;
}
