#include <stdlib.h>

#define IS_DIGIT(x) ((x) >= '0' && (x) <= '9')

int
atoi(const char *s)
{
  int n = 0;
  int sign = 1;

  if (*s == '-' || *s == '+')
    {
      sign = *s == '-' ? -1 : 1;
      s++;
    }

  while (IS_DIGIT(*s))
    n = n * 10 + *s++ - '0';

  return n * sign;
}
