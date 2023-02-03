#include <sys/x86.h>
#include <thread.h>
#include <stdio.h>


void*
thread_1(void *arg)
{
  for (;;)
    {
      printf("thread 1\n");
      thread_yield();
    }
}

void*
thread_2(void *arg)
{
  for (;;)
    {
      printf("thread 2\n");
      thread_yield();
    }
}

void*
init(void *arg)
{
  struct thread *t1, *t2;
  thread_create(&t1, "thread 1", &thread_1, NULL);
  thread_create(&t2, "thread 2", &thread_2, NULL);

  for (;;)
    ;
}
