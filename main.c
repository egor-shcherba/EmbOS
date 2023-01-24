#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <mem/heap.h>
#include <debug/qemu.h>

void
main(void)
{
  qprintf("kernel start initialize...\n");

  gdt_init();
  idt_init();
  pic_init();

  heap_init();

  for (;;)
    ;
}
