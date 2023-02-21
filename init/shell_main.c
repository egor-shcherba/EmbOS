#include <init/shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ARGS        64
#define ARG_NAME_LENGTH 256

static int
parse_args(char **args, const char *s)
{
  int n = 0;
  char arg[ARG_NAME_LENGTH];
  char *s2 = (char*) arg;

  while (*s == ' ' && *s != '\0')
    s++;

  if (*s == '\0')
    return n;

  while (*s != '\0')
    {
      if (*s == ' ')
        {
          s++;
          continue;
        }

      s2 = arg;

      while (*s != ' ' && *s != '\0')
        {
          if (*s == '"')
            {
              s++;

              while (*s != '"' && *s != '\0')
                *s2++ = *s++;
              s++;
              break;
             }

          *s2++ = *s++;
        }

      *s2 = '\0';
      args[n] = malloc(ARG_NAME_LENGTH);

      if (args[n] != NULL)
        strcpy(args[n], arg);

      n++;
    }

  return n;
}

static inline void
free_args(char **args)
{
  for (int i = 0; args[i] != NULL && i < MAX_ARGS; i++)
    free(args[i]);
}

void*
shell_main(void *arg)
{
  (void) arg;
  int ret_code;
  char input_str[BUFSIZ];
  struct thread *thread_cmd;
  struct cmd_entry *cmd = NULL;
  bool and_op = false;
  int and_op_pos = 0;

  printf("Welcome to EmbOS\n");

  char *args_array[MAX_ARGS];
  char **args = args_array;

  while (1)
    {
      memset(args, 0, sizeof(args));
      and_op = false;
      and_op_pos = 0;

      printf("$ ");
      gets(input_str);
      parse_args(args, input_str);

    if_and_op_true:

      for (int i = 0; i < MAX_ARGS && args[i] != NULL; i++)
        {
          if (strcmp(args[i], "&&") == 0)
            {
              and_op = true;
              args[i] = NULL;
              and_op_pos = i;
              break;
            }
        }

      if (args[0] == NULL)
        continue;

      cmd = shell_get_command(args[0]);

      if (cmd == NULL)
        {
          printf("%s: command not found\n", args[0]);
          continue;
        }

      thread_create(&thread_cmd, input_str, cmd->probe, (void*) args);
      thread_join(thread_cmd, (void*) &ret_code);

      if (ret_code == 0 && and_op == true)
        {
          and_op = false;
          args = args + and_op_pos + 1;
          goto if_and_op_true;
        }

      args = args_array;
      free_args(args);
    }

  return NULL;
}
