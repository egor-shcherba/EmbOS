#ifndef _THREAD_H
#define _THREAD_H

#include <system/syscall.h>
#include <stddef.h>

struct thread;

static inline unsigned int
__fn_obj_conv(void *(fn)(void*))
{
  unsigned int addr = (unsigned int) fn;
  return addr;
}

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
    (void*) __fn_obj_conv(start_routine),
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

static inline void
thread_wakeup(struct thread *thread)
{
  _syscall(SYS_thread_wakeup, thread, NULL, NULL, NULL, NULL);
}

static inline int
thread_join(struct thread *thread, void **arg)
{
  return _syscall(SYS_thread_join, thread, arg, NULL, NULL, NULL);
}

static inline void
thread_exit(void *retval)
{
  _syscall(SYS_thread_exit, retval, NULL, NULL, NULL, NULL);
}

#endif /* NOT _THREAD_H */
