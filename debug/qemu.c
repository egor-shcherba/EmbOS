#include <debug/qemu.h>
#include <klib/vsprintf.h>
#include <sys/io.h>
#include <stdarg.h>

#define QEMU_PORT   0xE9

#define BUFFER_SIZE 1024

static char buf[BUFFER_SIZE] = { 0x0 };

void
qprintf(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  int len;
  char *s = (char*) buf;

  len = vsprintf(buf, fmt, ap);
  va_end(ap);

  while (len--)
    outb(QEMU_PORT, *s++);
}
