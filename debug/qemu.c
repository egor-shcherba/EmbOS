#include <debug/qemu.h>
#include <klib/vsprintf.h>
#include <sys/io.h>
#include <stdarg.h>
#include <stddef.h>

#define QEMU_PORT   0xE9

#define BUFFER_SIZE 4096

static char buf[BUFFER_SIZE] = { 0x0 };

static inline void
dputs(const char *s)
{
  while (*s != '\0')
    outb(QEMU_PORT, *s++);
}

void
qprintf(const char *fmt, ...)
{
  va_list ap;
  int len;
  char *s = (char*) buf;

  va_start(ap, fmt);
  len = vsprintf(buf, fmt, ap);
  va_end(ap);

  while (len--)
    outb(QEMU_PORT, *s++);
}

void
dbg_printf(const char *header, const char *fmt, ...)
{
  if (header != NULL)
    qprintf("[%s] :: ", header);

  va_list ap;
  int len;
  char *s = (char*) buf;

  va_start(ap, fmt);
  len = vsprintf(buf, fmt, ap);
  va_end(ap);


  while (len--)
    outb(QEMU_PORT, *s++);
}
