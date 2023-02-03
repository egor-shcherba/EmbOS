#include <sys/idt.h>
#include <sys/exception.h>
#include <sys/irq.h>
#include <debug/qemu.h>
#include <stdint.h>
#include <string.h>

#define __DEBUG_HEADER__  "CPU"

extern void syscall_enter(void);

struct idt_entry {
  uint32_t handler_low : 16;
  uint32_t code_sel : 16;
  uint32_t zero : 8;
  uint32_t type : 5;
  uint32_t dpl : 2;
  uint32_t p : 1;
  uint32_t handler_high : 16;
};

struct idt_pointer {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

#define TRAP_GATE       0x0F
#define INTR_GATE       0x0E

#define IDT_TABLE_SIZE  256
static struct idt_entry idt_table[IDT_TABLE_SIZE] = { 0x0 };

static struct idt_pointer idt_pointer = { 0x0 };

static void
register_interrupt(struct idt_entry *entry, void (*handler)(void), uint32_t type)
{
  entry->handler_low  = (uint32_t) handler & 0xFFFF;
  entry->handler_high = (uint32_t) handler >> 16;
  entry->code_sel     = 0x8;
  entry->zero         = 0;
  entry->type         = type;
  entry->dpl          = 0;
  entry->p            = 1;
}

void
idt_init(void)
{
  for (int i = 0; i < IDT_TABLE_SIZE; i++)
    memset(&idt_table[i], 0, sizeof(struct idt_entry));

  register_interrupt(&idt_table[0], &exc_divide_error, TRAP_GATE);
  register_interrupt(&idt_table[1], &exc_debug, TRAP_GATE);
  register_interrupt(&idt_table[2], &exc_nmi, TRAP_GATE);
  register_interrupt(&idt_table[3], &exc_breakpoint, TRAP_GATE);
  register_interrupt(&idt_table[4], &exc_overflow, TRAP_GATE);
  register_interrupt(&idt_table[5], &exc_boundrange, TRAP_GATE);
  register_interrupt(&idt_table[6], &exc_inv_opcode, TRAP_GATE);
  register_interrupt(&idt_table[7], &exc_no_dev, TRAP_GATE);
  register_interrupt(&idt_table[8], &exc_double_fault, TRAP_GATE);
  register_interrupt(&idt_table[0xA], &exc_inv_tss, TRAP_GATE);
  register_interrupt(&idt_table[0xB], &exc_seg_not_present, TRAP_GATE);
  register_interrupt(&idt_table[0xC], &exc_ss, TRAP_GATE);
  register_interrupt(&idt_table[0xD], &exc_general_prot, TRAP_GATE);
  register_interrupt(&idt_table[0xE], &exc_page_fault, TRAP_GATE);
  register_interrupt(&idt_table[0x10], &exc_x87, TRAP_GATE);
  register_interrupt(&idt_table[0x11], &exc_aligned_check, TRAP_GATE);
  register_interrupt(&idt_table[0x12], &exc_machine_check, TRAP_GATE);
  register_interrupt(&idt_table[0x13], &exc_simd, TRAP_GATE);

  register_interrupt(&idt_table[0x20], &irq_0,  INTR_GATE);
  register_interrupt(&idt_table[0x21], &irq_1,  INTR_GATE);
  register_interrupt(&idt_table[0x22], &irq_2,  INTR_GATE);
  register_interrupt(&idt_table[0x23], &irq_3,  INTR_GATE);
  register_interrupt(&idt_table[0x24], &irq_4,  INTR_GATE);
  register_interrupt(&idt_table[0x25], &irq_5,  INTR_GATE);
  register_interrupt(&idt_table[0x26], &irq_6,  INTR_GATE);
  register_interrupt(&idt_table[0x27], &irq_7,  INTR_GATE);
  register_interrupt(&idt_table[0x28], &irq_8,  INTR_GATE);
  register_interrupt(&idt_table[0x29], &irq_9,  INTR_GATE);
  register_interrupt(&idt_table[0x2A], &irq_10, INTR_GATE);
  register_interrupt(&idt_table[0x2A], &irq_11, INTR_GATE);
  register_interrupt(&idt_table[0x2A], &irq_12, INTR_GATE);
  register_interrupt(&idt_table[0x2A], &irq_13, INTR_GATE);
  register_interrupt(&idt_table[0x2A], &irq_14, INTR_GATE);
  register_interrupt(&idt_table[0x2A], &irq_15, INTR_GATE);

  register_interrupt(&idt_table[0x80], &syscall_enter, INTR_GATE);

  idt_pointer.limit = sizeof(idt_table) - 1;
  idt_pointer.base  = (uint32_t) &idt_table;

  __asm__ volatile
  (
    "lidt %0" :: "m" (idt_pointer)
  );

  dprintf("idt initialized.\n");
}
