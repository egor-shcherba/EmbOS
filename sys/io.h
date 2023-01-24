#ifndef _IO_H
#define _IO_H

#include <stdint.h>

static inline void
outb(uint16_t port, uint8_t data)
{
  __asm__ volatile ("outb %0, %1" :: "a" (data), "d" (port));
}

static inline uint8_t
inb(uint16_t port)
{
  uint8_t data;

  __asm__ volatile ("inb %1, %0" : "=a" (data) : "d" (port));

  return data;
}

#endif /* NOT _IO_H */
