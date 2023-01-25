#ifndef _UART_H
#define _UART_H

#include <stdint.h>

void uart_init(void);
void uart_write(uint8_t byte);
uint8_t uart_read(void);

#endif /* NOT _UART_H */
