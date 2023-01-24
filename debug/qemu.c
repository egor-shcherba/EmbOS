#include <debug/qemu.h>
#include <klib/vsprintf.h>
#include <sys/io.h>
#include <stdarg.h>

#define QEMU_PORT   0xE9

#define BUFFER_SIZE 4096

static char buf[BUFFER_SIZE] = { 0x0 };

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
