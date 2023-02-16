#include <stdio.h>
#include <thread.h>
#include <unistd.h>

void*
thread_t1(void *arg)
{
  (void) arg;

  for (int i = 0; i < 2; i++)
    printf("thread t1 :: %c\n", getchar());

  return NULL;
}

void*
thread_t2(void *arg)
{
  (void) arg;

  for (int i = 0; i < 4; i++)
    printf("thread t2 :: %c\n", getchar());

  return NULL;
}

void*
init(void *arg)
{
  (void) arg;

  struct thread *t1, *t2;

  thread_create(&t1, "TEST", &thread_t1, NULL);
  thread_create(&t2, "TEST", &thread_t2, NULL);

  for (int i = 0; i < 5; i++)
    printf("thread main :: %c\n", getchar());

  for (;;)
    ;

  return NULL;
}
