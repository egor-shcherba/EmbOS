#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>


static void*
thread_routine(void *arg)
{
  (void) arg;

  int id = thread_id();
  printf("thread id %d :: sleep on %d seconds \n", id, id);
  sleep(id);
  printf("thread id %d :: wakeup \n", id);

  return EXIT_SUCCESS;
}

void*
cmd_thread_example0(void *arg)
{
  (void) arg;

  struct thread *threads[5];

  for (int i = 0; i < 5; i++)
    thread_create(&threads[i], "TEST", &thread_routine, &i);

  for (int i = 0; i < 5; i++)
    thread_join(threads[i], NULL);

  printf("thread example1 :: wait all\n");

  return EXIT_SUCCESS;
}
