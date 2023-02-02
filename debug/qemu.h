#ifndef _QEMU_H
#define _QEMU_H

#include <stddef.h>
void qprintf(const char *fmt, ...);

void dbg_printf(const char *header, const char *fmt, ...);

#ifndef __DEBUG_HEADER__
#define dprintf(...)                           \
  dbg_printf(__DEBUG_HEADER__, __VA_ARGS__)
#else
#define dprintf(...)                           \
  qprintf(__VA_ARGS__)
#endif  /* __DEBUG_HEADER__ */

#endif /* NOT QEMU_H */
