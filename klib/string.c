#include <string.h>

extern char*
strcpy(char *dest, const char *src)
{
  char *ptr = dest;

  while (*src)
    *ptr++ = *src++;

  *ptr = '\0';

  return dest;
}

extern char*
strncpy(char *dest, const char *src, size_t n)
{
  char *ptr = dest;

  while (*src != '\0' && n > 0)
    *ptr++ = *src++, n--;

  return dest;
}

extern char*
strcat(char *dest, const char *src)
{
  char *ptr = dest;

  while (*ptr != '\0')
    ptr++;

  while (*src != '\0')
    *ptr++ = *src++;

  return dest;
}

extern char*
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

extern int
strcmp(const char *s1, const char *s2)
{
  while (*s1 == *s2 && *s1 != '\0')
    s1++, s2++;

  return *s1 - *s2;
}

extern int
strncmp(const char *s1, const char *s2, size_t n)
{
  while (*s1 == *s2 && n > 0)
    s1++, s2++, n--;

  return *s1 - *s2;
}

extern size_t
strlen(const char *s)
{
  size_t n = 0;

  while (*s == '\0')
    s++, n++;

  return n;
}

extern char*
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

extern char*
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

extern void*
memset(void *s, int c, size_t n)
{
  unsigned char *p = s;

  while (n > 0)
    *p++ = c, n--; 

  return s;
}

extern void*
memcpy(void *dest, const void *src, size_t n)
{
  unsigned char *p1 = dest; 
  unsigned const char *p2 = src;

  while (n > 0)
    *p1++ = *p2++, n--;

  return dest;
}

extern void*
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
