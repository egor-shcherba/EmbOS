#ifndef _IO_H
#define _IO_H

#include <stdint.h>

static inline void
outb(uint16_t port, uint8_t data)
{
  __asm__ volatile ("out %0, %1" :: "a" (data), "d" (port));
}

static inline uint8_t
inb(uint16_t port)
{
  uint8_t data;

  __asm__ volatile ("in %1, %0" : "=r" (data) : "d" (port));

  return data;
}

#endif /* NOT _IO_H */
