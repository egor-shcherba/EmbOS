#ifndef _STDIO_H
#define _STDIO_H

#include <driver/uart.h>

#define putchar(x)  uart_write(x)

void printf(const char *format, ...);


#endif /* NOT _STDIO_H */
