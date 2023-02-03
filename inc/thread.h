#ifndef _THREAD_H
#define _THREAD_H

#include <system/syscall.h>
#include <stddef.h>

struct thread;

static inline int
thread_create(
  struct thread **thread,
  const char *name,
  void *(*start_routine)(void*),
  void *arg)
{
  return _syscall(
    SYS_thread_create, thread,
    (void*) name,
    (void*) start_routine,
    arg,
    NULL
  );
}

static inline void
thread_yield(void)
{
  _syscall(SYS_thread_yield, NULL, NULL, NULL, NULL, NULL);
}

static inline void
thread_sleep(void)
{
  _syscall(SYS_thread_sleep, NULL, NULL, NULL, NULL, NULL);
}

#endif /* NOT _THREAD_H */
