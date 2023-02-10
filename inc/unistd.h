#ifndef _UNISTD_H
#define _UNISTD_H

#include <system/syscall.h>

static inline int
sleep(unsigned seconds)
{
  return _syscall(SYS_settimer, (void*) (seconds * 1000), NULL, NULL, NULL, NULL);
}

#endif  /* NOT _UNISTD_H */
