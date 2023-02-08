#include <sys/x86.h>
#include <thread.h>
#include <stdio.h>
#include <macros.h>
#include <sys/io.h>
#include <debug/qemu.h>

struct thread *threads[5];

struct thread_mutex *mutex;

void*
thread_test(void *arg)
{
  (void) arg;

  thread_mutex_lock(&mutex);

  for (int i = 0; i < 0x1000000; i++)
    outb(0x80, i);

  printf("thread id %d\n", thread_id());

  thread_mutex_unlock(&mutex);

  return NULL;
}

void*
init(void *arg)
{
  UNUSED(arg);

  thread_mutex_init(&mutex);

  for (int i = 0; i < 5; i++)
    thread_create(&threads[i], "TEST", &thread_test, NULL);
 
  return NULL;
}
