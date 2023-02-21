#include <stdio.h>
#include <thread.h>
#include <unistd.h>
#include <init/shell.h>

static struct thread *thread_shell;

void*
init(void *arg)
{
  (void) arg;

  shell_init();

  thread_create(&thread_shell, "SHELL", &shell_main, NULL);

  return NULL;
}
