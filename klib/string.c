#include <string.h>

char*
strcpy(char *dest, const char *src)
{
  char *ptr = dest;

  while (*src)
    *ptr++ = *src++;

  *ptr = '\0';

  return dest;
}

char*
strncpy(char *dest, const char *src, size_t n)
{
  char *ptr = dest;

  while (*src != '\0' && n > 0)
    *ptr++ = *src++, n--;

  return dest;
}

char*
strcat(char *dest, const char *src)
{
  char *ptr = dest;

  while (*ptr != '\0')
    ptr++;

  while (*src != '\0')
    *ptr++ = *src++;

  return dest;
}

char*
strncat(char *dest, const char *src, size_t n)
{
  char *ptr = dest;

  while (*ptr != '\0')
    ptr++;

  while (*src != '\0' && n > 0)
    *ptr++ = *src++, n--;

  *ptr = '\0';

  return dest;
}

int
strcmp(const char *s1, const char *s2)
{
  while (*s1 == *s2 && *s1 != '\0')
    s1++, s2++;

  return *s1 - *s2;
}

int
strncmp(const char *s1, const char *s2, size_t n)
{
  while (*s1 == *s2 && n > 0)
    s1++, s2++, n--;

  return *s1 - *s2;
}

size_t
strlen(const char *s)
{
  char *s2 = (char*) s;

  while (*s2++)
    ;

  return s2 - s;
}

char*
strchr(const char *s, int c)
{
  while (*s != '\0')
    {
      if (*s == c)
        return (char*) s;

      s++;
    }

  return NULL;
}

char*
strrchr(const char *s, int c)
{
  char *ptr = NULL;

  while (*s != '\0')
    {
      if (*s == c)
        ptr = (char*) s;

      s++;
    }

  return ptr;
}

void*
memset(void *s, int c, size_t n)
{
  unsigned char *p = s;

  while (n > 0)
    *p++ = c, n--; 

  return s;
}

void*
memcpy(void *dest, const void *src, size_t n)
{
  unsigned char *p1 = dest; 
  unsigned const char *p2 = src;

  while (n > 0)
    *p1++ = *p2++, n--;

  return dest;
}

void*
memmove(void *dest, const void *src, size_t n)
{
  unsigned char *p1 = dest;
  unsigned const char *p2 = src;

  if (p2 <= p1 && p2 + n > p1)
    {
      p1 += n;
      p2 += n;
      while (n > 0)
        *--p1 = *--p2, n--;
    }
  else
    {
      while (n > 0)
        *p1++ = *p2++, n--;
    }

  return dest;
}
