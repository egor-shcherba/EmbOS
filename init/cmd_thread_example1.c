#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>

static struct thread_mutex *mutex;

static void*
thread_routine(void *arg)
{
  (void) arg;

  thread_mutex_lock(&mutex);
  int id = thread_id();
  printf("thread id %d :: sleep on %d seconds \n", id, id);
  sleep(id);
  printf("thread id %d :: wakeup \n", id);
  thread_mutex_unlock(&mutex);

  return EXIT_SUCCESS;
}

void*
cmd_thread_example1(void *arg)
{
  (void) arg;

  struct thread *threads[5];

  thread_mutex_init(&mutex);

  for (int i = 0; i < 5; i++)
    thread_create(&threads[i], "TEST", &thread_routine, &i);

  for (int i = 0; i < 5; i++)
    thread_join(threads[i], NULL);

  printf("thread example1 :: wait all\n");

  thread_mutex_destroy(&mutex);

  return EXIT_SUCCESS;
}
