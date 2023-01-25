#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <mem/heap.h>
#include <driver/uart.h>
#include <debug/qemu.h>
#include <system/printf.h>

void
main(void)
{
  qprintf("kernel start initialize...\n");

  gdt_init();
  idt_init();
  pic_init();
  heap_init();
  uart_init();

  printf("Welcome to EmbOS...\n");

  for (;;)
    ;
}
