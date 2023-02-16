#include <driver/uart.h>
#include <sys/io.h>
#include <sys/pic.h>
#include <debug/qemu.h>
#include <macros.h>
#include <klib/ring.h>
#include <klib/list.h>
#include <system/thread.h>
#include <thread.h>
#include <system/sched.h>
#include <errno.h>

#define __DEBUG_HEADER__ "UART"

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

#define CAPACITY    2048    /* ring buffer capacity */

#define BACKSPACE   0x08

static unsigned char ascii_code[] = {
  [0x00] = '\0',  [0x20] = ' ',  [0x40] = '@',  [0x60] = '`',
  [0x01] = '\0',  [0x21] = '!',  [0x41] = 'A',  [0x61] = 'a',
  [0x02] = '\0',  [0x22] = '"',  [0x42] = 'B',  [0x62] = 'b',
  [0x03] = '\0',  [0x23] = '#',  [0x43] = 'C',  [0x63] = 'c',
  [0x04] = '\0',  [0x24] = '$',  [0x44] = 'D',  [0x64] = 'd',
  [0x05] = '\0',  [0x25] = '%',  [0x45] = 'E',  [0x65] = 'e',
  [0x06] = '\0',  [0x26] = '&',  [0x46] = 'F',  [0x66] = 'f',
  [0x07] = '\0',  [0x27] = '\'', [0x47] = 'G',  [0x67] = 'g',
  [0x08] = '\0',  [0x28] = '(',  [0x48] = 'H',  [0x68] = 'h',
  [0x09] = '\0',  [0x29] = ')',  [0x49] = 'I',  [0x69] = 'i',
  [0x0A] = '\0',  [0x2A] = '*',  [0x4A] = 'J',  [0x6A] = 'j',
  [0x0B] = '\0',  [0x2B] = '+',  [0x4B] = 'K',  [0x6B] = 'k',
  [0x0C] = '\0',  [0x2C] = ',',  [0x4C] = 'L',  [0x6C] = 'l',
  [0x0D] = '\n',  [0x2D] = '-',  [0x4D] = 'M',  [0x6D] = 'm',
  [0x0E] = '\0',  [0x2E] = '.',  [0x4E] = 'N',  [0x6E] = 'n',
  [0x0F] = '\0',  [0x2F] = '/',  [0x4F] = 'O',  [0x6F] = 'o',
  [0x10] = '\0',  [0x30] = '0',  [0x50] = 'P',  [0x70] = 'p',
  [0x11] = '\0',  [0x31] = '1',  [0x51] = 'Q',  [0x71] = 'q',
  [0x12] = '\0',  [0x32] = '2',  [0x52] = 'R',  [0x72] = 'r',
  [0x13] = '\0',  [0x33] = '3',  [0x53] = 'S',  [0x73] = 's',
  [0x14] = '\0',  [0x34] = '4',  [0x54] = 'T',  [0x74] = 't',
  [0x15] = '\0',  [0x35] = '5',  [0x55] = 'U',  [0x75] = 'u',
  [0x16] = '\0',  [0x36] = '6',  [0x56] = 'V',  [0x76] = 'v',
  [0x17] = '\0',  [0x37] = '7',  [0x57] = 'W',  [0x77] = 'w',
  [0x18] = '\0',  [0x38] = '8',  [0x58] = 'X',  [0x78] = 'x',
  [0x19] = '\0',  [0x39] = '9',  [0x59] = 'Y',  [0x79] = 'y',
  [0x1A] = '\0',  [0x3A] = ':',  [0x5A] = 'Z',  [0x7A] = 'z',
  [0x1B] = '\0',  [0x3B] = ';',  [0x5B] = '[',  [0x7B] = '{',
  [0x1C] = '\0',  [0x3C] = '<',  [0x5C] = '\\', [0x7C] = '|',
  [0x1D] = '\0',  [0x3D] = '=',  [0x5D] = ']',  [0x7D] = '|',
  [0x1E] = '\0',  [0x3E] = '>',  [0x5E] = '^',  [0x7E] = '~',
  [0x1F] = '\0',  [0x3F] = '?',  [0x5F] = '_',  [0x7F] = ' ',
};

static struct ring_buffer rb;
static struct list waiters;

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

void
uart_handler(void)
{
  uint8_t byte = 0;
  struct thread *thread = NULL;

  for (;;)
    {
      byte = inb(COM1_PORT + REG_LSR);

      if (!(byte & READY))
        break;

      byte = inb(COM1_PORT + REG_DATA);

      if (list_empty(&waiters))
        return;

      if (byte == BACKSPACE)
        {
          if (ring_buffer_is_empty(&rb))
            return;

          char esc[] = { 0x1b, 0x5b, 0x44, 0x20, 0x1b, 0x5b, 0x44, 0x0 };

          for (char *c = esc; *c != 0; c++)
            uart_write(*c);

          ring_buffer_read(&rb);
          return;
        }

      if (!ring_buffer_is_full(&rb))
        {
          if (ascii_code[byte] == 0)
            return;

          uart_write(ascii_code[byte]);
          ring_buffer_write(&rb, ascii_code[byte]);
        }

      if (byte == 0xd)
        {
          while (!list_empty(&waiters))
            {
              thread = list_first_entry(&waiters, struct thread, node);
              list_remove(&thread->node);
              sys_thread_wakeup(thread);
              dprintf("remove waiter '%s' id %d\n", thread->name, thread->id);
            }
        }
    }
}

void
uart_init(void)
{
  outb(COM1_PORT + REG_LSR,  DLAB);
  outb(COM1_PORT + REG_DIVL, DIVISOR & 0xF);
  outb(COM1_PORT + REG_DIVH, DIVISOR >> 8);
  outb(COM1_PORT + REG_LCR,  BIT_MODE | PARITY_NONE | STOP_BIT);
  outb(COM1_PORT + REG_IER,  IER_DATA);

  irq_set_handler(4, &uart_handler);
  ring_buffer_init(&rb, CAPACITY);
  list_init(&waiters);

  dprintf("uart initialized.\n");
}

int
sys_read(void)
{
  struct thread *thread = (struct thread*) sys_thread_self();

  if (ring_buffer_is_empty(&rb))
    {
      list_remove(&thread->node);
      list_add_tail(&waiters, &thread->node);
      sys_thread_sleep();
      dprintf("add waiter '%s' id %d\n", thread->name, thread->id);

      return -EAGAIN;
    }

  return ring_buffer_read(&rb);
}

int
sys_flush(void)
{
  ring_buffer_flush(&rb);

  return 0;
}
