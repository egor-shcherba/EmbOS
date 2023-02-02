#include <stdio.h>
#include <driver/uart.h>
#include <klib/vsprintf.h>
#include <sys/x86.h>

#include <stdarg.h>

#define BUFSIZE  4096

void
printf(const char *format, ...)
{
  char buf[BUFSIZE];
  char *s = buf;
  va_list ap;
  int len = 0;

  va_start(ap, format);
  len = vsprintf(buf, format, ap);
  va_end(ap);

  while (len--)
    uart_write(*s++);
}
