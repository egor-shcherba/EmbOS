#ifndef _SCHED_H
#define _SCHED_H

struct thread;

int sys_thread_create(
  struct thread **thread,
  const char *name,
  void *(**start_routine)(void *arg),
  void *arg);

int sys_thread_yield(void);
int sys_thread_sleep(void);
int sys_thread_wakeup(struct thread *thread);

void sched_init(void);

void sched_enable(void);

#endif /* NOT _SCHED_H */
