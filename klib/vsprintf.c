#include <klib/vsprintf.h>
#include <string.h>

#define FLAG_LEFT     0x0001
#define FLAG_UPPER    0x0002
#define FLAG_ZEROPAD  0x0004
#define FLAG_SPACE    0x0008
#define FLAG_SIGN     0x0010
#define FLAG_PLUS     0x0020
#define FLAG_HASH     0x0040

#define IS_DIGIT(c) ((c) >= '0' && ((c) <= '9'))

static char *lower_digit = "0123456789abcdef";
static char *upper_digit = "0123456789ABCDEF";

static char *null_str = "(null)";
static char *null_ptr = "(nil)";

static inline char*
number(char *s, long num, long base, long width, long precision, long flags)
{
  int i = 0;
  char tmp[64] = { 0x0 };
  char sign = '\0';
  unsigned long number = num;
  char *digit = lower_digit;
  char c = (flags & FLAG_ZEROPAD) ? '0': ' ';
  char *hash_str = (void*) 0;

  if (flags & FLAG_LEFT)
    flags &= ~FLAG_ZEROPAD;

  if (flags & FLAG_UPPER)
    digit = upper_digit;

  if (num < 0 && (flags & FLAG_SIGN))
    {
      number = ~num + 1;
      sign = '-';
    }
  else
    sign = (flags & FLAG_PLUS) ? '+' : (flags & FLAG_SPACE) ? ' ' : 0;

  if (base != 10)
    sign = '\0';

  if (flags & FLAG_HASH)
    {
      switch (base)
        {
          case 8:
            width--;
            hash_str = "0";
            break;
          case 16:
            width -= 2;
            hash_str = "0x";
            break;
        }
    }

  if (number == 0)
    tmp[i++] = '0';
  else
    {
      while (number != 0)
        {
          tmp[i++] = digit[number % base];
          number /= base;
        }
    }

  if (precision < i)
    precision = i;

  width -= precision;

  if (!(flags & (FLAG_LEFT + FLAG_ZEROPAD)))
    {
      while (width-- > 0)
        *s++ = ' ';
    }

  if (sign)
    *s++ = sign;

  if (hash_str)
    {
      while (*hash_str)
        *s++ = *hash_str++;
    }

  if (!(flags & FLAG_LEFT))
    {
      while (width--> 0)
        *s++ = c;
    }

  while (precision-- > i)
    *s++ = '0';

  while (i--)
    *s++ = tmp[i];

  while (width-- > 0)
    *s++ = ' ';

  return s;
}

static inline int
skip_atoi(char **s)
{
  int n = 0;

  while (IS_DIGIT(**s))
    n = n * 10 + *(*s)++ - '0';

  return n;
}

int
sprintf(char *buf, const char *format, ...)
{
  int len;
  va_list args;
  va_start(args, format);

  len = vsprintf(buf, format, args);

  va_end(args);

  return len;
}

int
vsprintf(char *buf, const char *format, va_list ap)
{
  char *s, *str;
  int len = 0;
  int flags = 0;
  int width = 0;
  int precision = 0;
  unsigned long val = 0;
  char c = 0;
  int base = 10;
  char *prev_fmt_ptr = 0;
  char qualifier = -1;

  for (s = buf; *format; format++)
    {
      if (*format != '%')
        {
          *s++ = *format;
          continue;
        }

      prev_fmt_ptr = (char*) format;

      repeat:
        format++;
        switch (*format)
          {
             case '+':
              flags |= FLAG_PLUS;
              goto repeat;
             case '-':
              flags |= FLAG_LEFT;
              goto repeat;
             case '0':
              flags |= FLAG_ZEROPAD;
              goto repeat;
             case ' ':
              flags |= FLAG_SPACE;
              goto repeat;
             case '#':
              flags |= FLAG_HASH;
              goto repeat;
          }

      if (IS_DIGIT(*format))
        width = skip_atoi((char**) &format);

      if (*format == '.')
        {
          format++;
          precision = skip_atoi((char**) &format);
        }

      if (strchr("lh", *format) != 0)
        {
          qualifier = *format++;

          if (qualifier == 'h' && *format == 'h')
            {
              qualifier = 'c';
              format++;
            }
        }

      switch (*format)
        {
          case 'i': case 'l': case 'd':
            flags |= FLAG_SIGN;
            break;

          case 'u':
            break;

          case 'x':
            base = 16;
            break;

          case 'X':
            base = 16;
            flags |= FLAG_UPPER;
            break;

          case 'p':
            val = (unsigned long) va_arg(ap, long);

            if (val == 0)
              {
                str = (char*) null_ptr;
                precision = strlen(null_ptr);
                goto print_null_str;
              }

            s = number(s, val, 16, width, sizeof(void*) * 2, flags | FLAG_HASH);
            break;

          case 's':
            str = va_arg(ap, char*);
            precision = -1;
            if (str == 0)
              {
                str = null_str;
                precision = strlen(null_str);
              }

          print_null_str:
            len = strlen(str);

            if (!(flags & FLAG_LEFT))
              {
                while (width-- > 0)
                  *s++ = ' ';
              }

            if (precision < len)
              precision = len;

            while (precision--)
              *s++ = *str++;

            while (width-- > 0)
              *s++ = ' ';

            continue;;

          case 'c':
            c = (char) va_arg(ap, int);

            if (!(flags & FLAG_LEFT))
              {
                while (width-- > 0)
                  *s++ = ' ';
              }

            *s++ = c;

            while (width-- > 0)
              *s++ = ' ';

            continue;

          case '%':
            *s++ = '%';
            break;

          default:
            *s++ = '%';
            format = prev_fmt_ptr;
        }

      if (flags & FLAG_SIGN)
        {
          if (qualifier == 'l')
            val = (long) va_arg(ap, long);
          else if (qualifier == 'h')
            val = (short) va_arg(ap, long);
          else if (qualifier == 'c')
            val = (char) va_arg(ap, long);
          else
            val = (int) va_arg(ap, long);
        }
      else
        {
          if (qualifier == 'l')
            val = (unsigned long) va_arg(ap, long);
          else if (qualifier == 'h')
            val = (unsigned short) va_arg(ap, long);
          else if (qualifier == 'c')
            val = (unsigned char) va_arg(ap, long);
          else
            val = (unsigned int) va_arg(ap, long);
        }

        s = number(s, val, base, width, precision, flags);
    }

  *s = '\0';

  return s - buf;
}
