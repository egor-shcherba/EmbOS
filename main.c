#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <mem/heap.h>
#include <driver/uart.h>
#include <debug/qemu.h>
#include <stdio.h>

#define __DEBUG_HEADER__  "KERNEL"

void
main(void)
{
  dprintf("kernel start initialize...\n");

  gdt_init();
  idt_init();
  pic_init();
  heap_init();
  uart_init();

  dprintf("initialized\n");

  printf("Welcome to EmbOS...\n");

  for (;;)
    ;
}
