#ifndef _UNISTD_H
#define _UNISTD_H

#include <system/syscall.h>

static inline int
sleep(unsigned seconds)
{
  return _syscall(SYS_settimer, (void*) (seconds * 1000), NULL, NULL, NULL, NULL);
}

static inline int
msleep(unsigned mseconds)
{
  return _syscall(SYS_settimer, (void*) (mseconds), NULL, NULL, NULL, NULL);
}

#endif  /* NOT _UNISTD_H */
