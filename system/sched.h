#ifndef _SCHED_H
#define _SCHED_H

struct thread;

int sys_thread_create(
  struct thread **thread,
  const char *name,
  void *(*start_routine)(void *arg),
  void *arg);

int sys_thread_yeald(void);
int sys_thread_sleep(void);

void sched_init(void);

#endif /* NOT _SCHED_H */
