#include <driver/uart.h>
#include <sys/io.h>
#include <debug/qemu.h>
#include <macros.h>

#define UART_CLOCK  115200
#define BAUD_RATE   57600
#define DIVISOR (UART_CLOCK / BAUD_RATE)

#define COM1_PORT   0x3F8   /* COM1 port */

#define DLAB        0x80
#define REG_LCR     3       /* line control register */
#define REG_LSR     5       /* line status register */

#define BIT_MODE    3       /* character length 8 bits */
#define PARITY_NONE 0       /* no parity bit will be added and non will be expected */
#define STOP_BIT    0       /* send 1 bit after each character data */

#define REG_DIVL    0       /* register low divisor */
#define REG_DIVH    1       /* register high divisor */

#define REG_IER     1       /* interrupt enable register */
#define IER_DATA    1       /* transmitter empty */

#define REG_DATA    0       /* data register */

#define EMPTY       0x20    /* trasmitted empty*/
#define READY       0x01    /* data ready */

void
uart_init(void)
{
  outb(COM1_PORT + REG_LSR,  DLAB);
  outb(COM1_PORT + REG_DIVL, DIVISOR & 0xF);
  outb(COM1_PORT + REG_DIVH, DIVISOR >> 8);
  outb(COM1_PORT + REG_LCR,  BIT_MODE | PARITY_NONE | STOP_BIT);
  outb(COM1_PORT + REG_IER,  IER_DATA);

  qprintf("[DRIVER] :: uart initialized.\n");
}

void
uart_write(uint8_t byte)
{
  while (1)
    {
       unsigned char data = inb(COM1_PORT + REG_LSR);

        if (data & EMPTY)
          break;
    }

  if (byte == '\n')
    outb(COM1_PORT + REG_DATA, '\r');

  outb(COM1_PORT + REG_DATA, byte);
}

uint8_t
uart_read(void)
{
  UNIMPLEMENTED;
  return 0x0;
}
