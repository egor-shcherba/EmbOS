#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/pit.h>
#include <mem/heap.h>
#include <driver/uart.h>
#include <system/sched.h>
#include <system/timer.h>
#include <debug/qemu.h>
#include <klib/list.h>

#define __DEBUG_HEADER__  "KERNEL"

extern void *(*init)(void*);

void
main(void)
{
  dprintf("kernel start initialize...\n");

  gdt_init();
  idt_init();
  pic_init();
  pit_init();
  heap_init();
  sched_init();
  timer_init();
  uart_init();

  dprintf("all subsytem initialized.\n");

  struct thread *thread_main;
  sys_thread_create(&thread_main, "init thread", &init, NULL);

  sched_enable();
}
