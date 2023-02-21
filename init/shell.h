#ifndef _SHELL_H
#define _SHELL_H

#define SHELL_NAME_LENGTH   64

typedef void *(fn_cmd)(void *);

struct cmd_entry {
  char name[SHELL_NAME_LENGTH];
  fn_cmd *probe;
};

void shell_init(void);
void* shell_main(void* arg);
void shell_table_init(void);

struct cmd_entry *shell_get_command(const char *cmd_name);
void shell_register_command(const char *cmd, void*(*cmd_fn)(void*));

#endif /* NOT _SHELL_H */
