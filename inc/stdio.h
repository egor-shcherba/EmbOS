#ifndef _STDIO_H
#define _STDIO_H

#include <driver/uart.h>

#define putchar(x)  uart_write(x)
#define BUFSIZ      4096

void printf(const char *format, ...);

int getchar(void);
char *gets(char *s);

#endif /* NOT _STDIO_H */
