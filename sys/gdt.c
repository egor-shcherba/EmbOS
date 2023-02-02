#include <sys/gdt.h>
#include <debug/qemu.h>
#include <stdint.h>

#define __DEBUG_HEADER__  "CPU"

struct gdt_entry {
  uint32_t limit_low : 16;
  uint32_t base_low  : 16;
  uint32_t base_mid  : 8;
  uint32_t type : 4;
  uint32_t s : 1;
  uint32_t dpl : 2;
  uint32_t p : 1;
  uint32_t limit_high : 4;
  uint32_t avl : 1;
  uint32_t l : 1;
  uint32_t d : 1;
  uint32_t g : 1;
  uint32_t base_high : 8;
};

struct gdt_pointer {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

#define CODE_TYPE       0x0A
#define DATA_TYPE       0x02

#define GDT_TABLE_SIZE  3
static struct gdt_entry gdt_table[GDT_TABLE_SIZE] = { 0x0 };
static struct gdt_pointer gdt_pointer = { 0x0 };

static void
gdt_set_gate(struct gdt_entry *entry, uint32_t base, uint32_t limit, uint32_t type)
{
  entry->limit_low  = limit & 0xFFFF;
  entry->limit_high = (limit >> 16) & 0xFF;
  entry->base_low   = base & 0xFFFFF;
  entry->base_mid   = (base >> 16) && 0xFF;
  entry->base_high  = (base >> 24) && 0xFF;
  entry->type       = type & 0xF;
  entry->s          = 1;
  entry->dpl        = 0;
  entry->p          = 1;
  entry->avl        = 0;
  entry->l          = 0;
  entry->d          = 1;
  entry->g          = 1;
}

void
gdt_init(void)
{
  /* NULL DESCRIPTOR */
  gdt_set_gate(&gdt_table[0], 0, 0, 0);

  /* CODE DESCRIPTOR */
  gdt_set_gate(&gdt_table[1], 0, 0xFFFFFF, CODE_TYPE);

  /* DATA DESCRIPTOR */
  gdt_set_gate(&gdt_table[2], 0, 0xFFFFFF, DATA_TYPE);

  gdt_pointer.limit = sizeof(gdt_table) - 1;
  gdt_pointer.base  = (uint32_t) &gdt_table;

  __asm__ volatile
  (
    "lgdt %0;"
    "mov $0x10, %%eax;"
    "mov %%eax, %%ss;"
    "mov %%eax, %%es;"
    "mov %%eax, %%ds;"
    "mov %%eax, %%gs;"
    "mov %%eax, %%fs;"
    "ljmp $0x8, $1f;"
    "1:"
    :: "m" (gdt_pointer)
  );

  dprintf("gdt initialized.\n");
}
