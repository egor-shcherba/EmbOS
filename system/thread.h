#ifndef _SYS_THREAD_H
#define _SYS_THREAD_H

#include <klib/list.h>

#define THREAD_NAME_LENGTH  64

enum THREAD_STATE {
  UNUSED,
  RUNNING,
  SLEEP,
  INTERUPTIBLE,
  DEAD
};

struct thread {
  int id;
  char name[THREAD_NAME_LENGTH];
  void *stackp;
  enum THREAD_STATE state;
  struct thread *joiner;
  void **retval;

  struct list node;
};

#endif /* NOT _SYS_THREAD_H */
