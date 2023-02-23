#include <init/shell.h>
#include <debug/qemu.h>
#include <stdio.h>

#define __DEBUG_HEADER__  "SHELL"

extern void* cmd_sleep(void *arg);
extern void* cmd_echo(void *arg);
extern void* cmd_uptime(void *arg);
extern void* cmd_uname(void *arg);

extern void* cmd_thread_example0(void *arg);
extern void* cmd_thread_example1(void *arg);
extern void* cmd_ud2(void *arg);

static char *help_msg =
  "sleep [NUMBER]\n"
  "echo [args...]\n"
  "uptime\n"
  "uname\n"
  "thread-example0\n"
  "thread-example1\n"
  "ud2\n";

void*
cmd_help(void *args)
{
  (void) args;

  printf("%s", help_msg);

  return NULL;
}

void
shell_init(void)
{
  shell_table_init();
  shell_register_command("sleep", cmd_sleep);
  shell_register_command("help", cmd_help);
  shell_register_command("echo", cmd_echo);
  shell_register_command("uptime", cmd_uptime);
  shell_register_command("uname", cmd_uname);
  shell_register_command("thread-example0", cmd_thread_example0);
  shell_register_command("thread-example1", cmd_thread_example1);
  shell_register_command("ud2", cmd_ud2);

  dprintf("cmd list initialized\n");
}
