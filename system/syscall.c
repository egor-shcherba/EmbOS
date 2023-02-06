#include <sys/x86.h>
#include <debug/qemu.h>
#include <system/syscall.h>
#include <system/sched.h>

#define __DEBUG_HEADER__ "SYSCALL"

static const char*
get_syscall_name(int num)
{
  switch (num)
    {
      case SYS_thread_create : return "sys_thread_create";
      case SYS_thread_yield  : return "sys_thread_yield";
      case SYS_thread_sleep  : return "sys_thread_sleep";
      case SYS_thread_wakeup : return "sys_thread_wakeup";
      case SYS_thread_join   : return "sys_thread_join";
      case SYS_thread_exit   : return "sys_thread_exit";
    }

  return "UNKNOW";
}

int
_syscall(int num, void *a1, void *a2, void *a3, void *a4, void *a5)
{
  int ret_val = 0;
  __asm__ volatile
  (
    "movl %1, %%eax;"
    "movl %2, %%ebx;"
    "movl %3, %%ecx;"
    "movl %4, %%edx;"
    "movl %5, %%esi;"
    "movl %6, %%edi;"
    "int $0x80;"
    : "=a" (ret_val)
    : "a" (num), "b" (a1), "c" (a2), "d" (a3), "S" (a4), "D" (a5)
  );

  return ret_val;
}

int
syscall_main(struct regs_t *regs)
{
  dprintf("call %d (%s) \n", regs->eax, get_syscall_name(regs->eax));

  switch (regs->eax)
    {
      case SYS_thread_create: return sys_thread_create(
        (void*) regs->ebx, (void*) regs->ecx,
        (void*(**)(void*)) regs->edx, (void*) regs->edi);

      case SYS_thread_yield: return sys_thread_yield();
      case SYS_thread_sleep: return sys_thread_sleep();
      case SYS_thread_wakeup: return sys_thread_wakeup((struct thread*) regs->ebx);

      case SYS_thread_join: return sys_thread_join(
        (struct thread*) regs->ebx, (void**) regs->ecx);

      case SYS_thread_exit: return sys_thread_exit((void*) regs->ebx);
    }

  return -1;
}
