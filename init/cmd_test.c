#include <stdio.h>
#include <stddef.h>
#include <thread.h>
#include <unistd.h>

void*
thread_example(void *arg)
{
  (void) arg;

  printf("thread example start\n");
  sleep(2);
  printf("thread example end\n");

  return NULL;
}

void*
cmd_test(void *arg)
{
  (void) arg;

  struct thread *t1;
  thread_create(&t1, "thread example", &thread_example, NULL);

  return NULL;
}
