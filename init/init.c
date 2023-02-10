#include <stdio.h>
#include <thread.h>
#include <unistd.h>

struct thread *threads[5];
struct thread_mutex *mutex;

void*
thread_test(void *arg)
{
  (void) arg;

  thread_mutex_lock(&mutex);

  printf("thread id %d sleep\n", thread_id());
  sleep(thread_id());
  printf("thread id %d wakeup\n", thread_id());

  thread_mutex_unlock(&mutex);

  return NULL;
}

void*
init(void *arg)
{
  (void) arg;

  thread_mutex_init(&mutex);

  for (int i = 0; i < 5; i++)
    thread_create(&threads[i], "TEST", &thread_test, NULL);

  return NULL;
}
