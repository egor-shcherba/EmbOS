#include <klib/vsscanf.h>
#include <stdio.h>
#include <stddef.h>

#define IS_DIGIT(c) ((c) >= '0' && ((c) <= '9'))

#define FLAG_SIGN 0x1

static inline int
skip_atoi(char **s)
{
  int n = 0;

  while (IS_DIGIT(**s))
    n = n * 10 + *(*s)++ - '0';

  return n;
}

static inline int
read_number(char **ptr)
{
  int n = 0;
  int sign = 1;
  char *s = *ptr;

  if (*s == '-' || *s == '+')
    {
      if (*s == '-')
        sign = -1;

      s++;
    }

  while (IS_DIGIT(*s))
    n = n * 10 + *s++ - '0';

  *ptr = s;

  return n * sign;
}

int
sscanf(char *buf, const char *format, ...)
{
  int err = 0;
  va_list ap;

  va_start(ap, format);
  err = vsscanf(buf, format, ap);
  va_end(ap);

  return err;
}

static void
strcpy(char *dest, char **src)
{
  while (**src != '\0')
    *dest++ = *(*src)++;
}

static void
strncpy(char *dest, char **src, int n)
{
  while (**src != '\0' && n-- > 0)
    *dest++ = *(*src)++;
}

int
vsscanf(char *buf, const char *format, va_list ap)
{
  char *last = NULL;
  void *p = NULL;
  int flags = 0;
  int width = 0;

  while (*format != 0)
    {
      if (*format != '%')
        {
          if (*format == *buf)
            {
              format++;
              buf++;
              continue;
            }

          return -1;
        }

      format++;
      last = (char*) format;
      width = skip_atoi((char**) &format);

      switch (*format)
        {
          case 'd':
            flags |= FLAG_SIGN;
            break;

          case 'u':
            break;

          case 'c':
            p = va_arg(ap, char*);
            *(char*) p = *buf++;
            format++;
            continue;

          case 's':
            p = va_arg(ap, char*);

            if (width == 0)
              strcpy((char*) p, &buf);
            else
              strncpy((char*) p, &buf, width);

            format++;
            continue;

          default:
            if (*buf != '%')
              return -1;

            format = last;
            buf++;
            continue;
        }

      format++;
      p = va_arg(ap, void*);

      if (flags & FLAG_SIGN)
        *(long*) p = read_number(&buf);
      else
        *(unsigned long*) p = read_number(&buf);
    }

  return -(*--format == *buf);
}
