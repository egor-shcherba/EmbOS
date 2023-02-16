#include <stdio.h>
#include <stddef.h>
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

void
fflush(void)
{
  _syscall(SYS_flush, NULL, NULL, NULL, NULL, NULL);
}
