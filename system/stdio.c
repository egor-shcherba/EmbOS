#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <klib/vsprintf.h>
#include <klib/vsscanf.h>
#include <system/syscall.h>

int
getchar(void)
{
  int c = -1;
  do
    {
      c = _syscall(SYS_read, NULL, NULL, NULL, NULL, NULL);
    }
  while (c == -1);

  return c;
}

char*
gets(char *s)
{
  int c = 0;

  while ((c = getchar()) != '\n')
    *s++ = c;

  *s++ = '\0';

  return s;
}

int
scanf(const char *format, ...)
{
  char buf[BUFSIZ];
  int err = 0;
  va_list ap;

  gets(buf);

  va_start(ap, format);
  err = vsscanf(buf, format, ap);
  va_end(ap);

  return err;
}

void
fflush(void)
{
  _syscall(SYS_flush, NULL, NULL, NULL, NULL, NULL);
}

void
puts(const char *s)
{
  while (*s)
    putchar(*s++);
}

void
printf(const char *format, ...)
{
  char buf[BUFSIZ];
  char *s = buf;
  va_list ap;
  int len = 0;

  va_start(ap, format);
  len = vsprintf(buf, format, ap);
  va_end(ap);

  while (len--)
    uart_write(*s++);
}
