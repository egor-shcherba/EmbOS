#ifndef _ASSERT_H
#define _ASSERT_H

#include <sys/x86.h>

#define assert(expr)                                  \
{                                                     \
  if (__builtin_expect(!!(!(expr)), 0))               \
    {                                                 \
      cpu_disable_interrupt();                        \
      qprintf("%s:%d: %s: Assertion '%s' failed.\n",  \
        __FILE__, __LINE__, __func__, #expr);         \
    }                                                 \
}

#endif /* NOT _ASSERT_H */
