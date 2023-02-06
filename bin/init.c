#include <sys/x86.h>
#include <thread.h>
#include <stdio.h>
#include <macros.h>

#include <debug/qemu.h>

struct thread *t1, *t2;

void*
thread_2(void *arg)
{
  UNUSED(arg);

  printf("thread 2\n");

  thread_exit( (void*) -14);

  return (void*) -2;
}

void*
thread_1(void *arg)
{
  UNUSED(arg);

  printf("thread 1\n");

  int retval = 0;
  int t_arg = 256;

  thread_create(&t2, "thread 2", &thread_2, (void*) &t_arg);

  thread_join(t2, (void**) &retval);
  printf("thread 2 exit retval %d\n", retval);


  thread_exit( (void*) -24);
  printf("thread 1 dead\n");

  return (void*) 1;
}

void*
init(void *arg)
{
  UNUSED(arg);

  printf("init thread\n");
  thread_create(&t1, "thread 1", &thread_1, NULL);

  int retval = 0;
  thread_join(t1, (void**) &retval);
  printf("thread 1 exit retval %d\n", retval);

  printf("init thread dead\n");
  thread_exit( (void*) 218);

  return NULL;
}
