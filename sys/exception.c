#include <debug/qemu.h>
#include <sys/x86.h>
#include <system/sched.h>
#include <system/thread.h>
#include <stdio.h>

static inline const char*
get_exception_name(int num)
{
  switch (num)
    {
      case 0x00: return "Divide By Zero";
      case 0x01: return "Debug";
      case 0x02: return "No Mascalable Interrupt";
      case 0x03: return "Breakpoint";
      case 0x04: return "Overflow";
      case 0x05: return "Bound-Range";
      case 0x06: return "Invalid Opcode";
      case 0x07: return "Device Not Avalible";
      case 0x08: return "Double Faults";
      case 0x0A: return "Invalid TSS";
      case 0x0B: return "Segment Not Present";
      case 0x0C: return "Stack Segment";
      case 0x0D: return "General Protection";
      case 0x0E: return "Page Faults";
      case 0x10: return "x87 Floating Point";
      case 0x11: return "Aligned Check";
      case 0x12: return "Machine Check";
      case 0x13: return "Simd";
    }

  return "UNKNOW";
}

void
exception_handler(struct regs_t *regs)
{
  qprintf("===========================================================\n");

  struct thread *thread = (struct thread*) sys_thread_self();

  if (thread != NULL)
    qprintf("THREAD '%s' id %d\n", thread->name, thread->id);
  else
    qprintf("!!!KERNEL PANIC!!!");

  qprintf("EXCEPTION '%s' error code %d\n",
    get_exception_name(regs->intr_number), regs->error_code);

  qprintf("EIP %#.8x CS  %#.8x EFLAGS %#.8x\n",
    regs->eip, regs->cs, regs->eflags);

  qprintf("EAX %#.8x EBX %#.8x ECX %#.8x EDX %#.8x\n",
    regs->eax, regs->ebx, regs->ecx, regs->edx);

  qprintf("===========================================================\n");

  if (thread != NULL)
    {
      printf("Segmentation fault\n");
      sys_thread_exit((void*) -1);
    }

  printf("KERNEL PANNIC: '%s'\n", get_exception_name(regs->intr_number);

  cpu_disable_interrupt();
  cpu_halt();
}
