#ifndef _STDIO_H
#define _STDIO_H

#include <driver/uart.h>

#define putchar(x)  uart_write(x)

#define BUFSIZ      4096

void printf(const char *format, ...);
void puts(const char *s);

int getchar(void);
char *gets(char *s);
int scanf(const char *format, ...);

#endif /* NOT _STDIO_H */
