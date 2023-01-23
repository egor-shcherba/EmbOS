#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

char* strcpy(char *dest, const char *src);
char* strncpy(char *dest, const char *src, size_t n);
char* strcat(char *dest, const char *src);
char* strncat(char *dest, const char *src, size_t n);

char* strchr(const char *s, int c);
char* strrchr(const char *s, int c);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(const char *s);

void* memset(void *s, int c, size_t n);
void* memcpy(void *dest, const void *src, size_t n);
void* memmove(void *dest, const void *src, size_t n);

#endif /* NOT _STRING_H */
