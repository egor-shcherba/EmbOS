#include <sys/x86.h>
#include <thread.h>
#include <stdio.h>
#include <macros.h>

#include <debug/qemu.h>

struct thread *t1;

void*
thread_1(void *arg)
{
  printf("thread 1 val %d\n", *(int*) arg);

  struct thread *self = thread_self();
  printf("thread self %.8x\n", self);
  printf("thread id %d\n", thread_id());

  return NULL;
}

void*
init(void *arg)
{
  UNUSED(arg);

  int val = 255;

  thread_create(&t1, "thread 1", &thread_1, &val);
  thread_join(t1, NULL);

  return NULL;
}
