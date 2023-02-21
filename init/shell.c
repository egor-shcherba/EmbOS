#include <init/shell.h>
#include <debug/qemu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __DEBUG_HEADER__ "SHELL"

#define SHELL_COMMAND_SIZE  128

static struct cmd_entry *cmd_table[SHELL_COMMAND_SIZE];

void
shell_table_init(void)
{
  memset(cmd_table, 0, sizeof(cmd_table));
}

int
hash(const char *cmd_name)
{
  int hash = 0;

  while (*cmd_name != '\0')
    hash += *cmd_name++;

  return hash;
}

#define UNREGISTER_COMMAND(cmd_name, msg)                         \
  {                                                               \
    dprintf("unregister command '%s' :: '%s'\n", cmd_name, msg);  \
    return;                                                       \
  }

void
shell_register_command(const char *cmd_name, void*(*cmd_fn)(void*))
{
  int h = hash(cmd_name) % SHELL_COMMAND_SIZE;
  struct cmd_entry **entry = &cmd_table[h];

  for (int i = 0; i < SHELL_COMMAND_SIZE && *entry != NULL; i++)
    {
      if (strcmp((*entry)->name, cmd_name) == 0)
        UNREGISTER_COMMAND(cmd_name, "command is exists");

      h = (h + i) % SHELL_COMMAND_SIZE;
      entry = &cmd_table[h];
    }

  if (*entry != NULL)
    UNREGISTER_COMMAND(cmd_name, "maximum commands rechead\n");

  *entry = malloc(sizeof(struct cmd_entry));

  if (*entry == NULL)
    UNREGISTER_COMMAND(cmd_name, "no mem");

  strcpy((*entry)->name, cmd_name);
  (*entry)->probe = cmd_fn;

  dprintf("register command '%s'\n", cmd_name);
}

struct cmd_entry*
shell_get_command(const char *cmd_name)
{
  int h = hash(cmd_name) % SHELL_COMMAND_SIZE;
  struct cmd_entry **entry = &cmd_table[h];

  for (int i = 0; i < SHELL_COMMAND_SIZE; i++)
    {
      if (strcmp((*entry)->name, cmd_name) == 0)
        return *entry;

      h = (h + i) % SHELL_COMMAND_SIZE;
      entry = &cmd_table[h];
    }

  return NULL;
}
