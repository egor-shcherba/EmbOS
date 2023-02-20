#include <mem/heap.h>
#include <debug/qemu.h>

#include <stdint.h>
#include <stddef.h>

#define __DEBUG_HEADER__  "MEM"

#define HEAP_SIZE   (0x00100000 * 16) /* 16 Mib */
static volatile uint8_t memory[HEAP_SIZE] = { 0x0 };

typedef struct header {
  size_t size;
  struct header *next;
  uint32_t flags;
  uint32_t pad;
} header_t;

header_t *freep = (header_t*) memory;

#define B2UNITS(x) (((x) + sizeof(header_t) - 1) / sizeof(header_t) + 1)

void
heap_init(void)
{
  size_t nunits = B2UNITS(HEAP_SIZE) - 1;
  freep->next = freep;
  freep->size = nunits;

  dprintf("heap size %u Mib\n", sizeof(memory) / 1024 / 1024);
  dprintf("heap initialized.\n");
}

void
heap_dump(void)
{
  size_t total_units = 0;
  header_t *p = (header_t*) memory;

  qprintf("===================================================\n");
  qprintf("HEAP DUMP:\n");

  while (total_units != B2UNITS(HEAP_SIZE) - 1)
    {
      qprintf("CHUNK %#.8x ", (void*) p);
      qprintf("SIZE %10d BYTE'S IS ", p->size * sizeof(header_t));

      if (p->next == NULL)
        qprintf("ALLOCATED\n");
      else
        qprintf("FREE\n");

      total_units += p->size;
      p += p->size;
    }

  qprintf("===================================================\n");
}

void*
kmalloc(size_t nbytes)
{
  size_t nunits = B2UNITS(nbytes);
  header_t *p = freep;

  do
    {
      if (p->size >= nunits)
        {
          if (p->size == nunits)
            freep = p->next == freep ? NULL : p->next;
          else
            {
              p->size -= nunits;
              p += p->size;
              p->size = nunits;
            }

          p->next = NULL;
          dprintf("allocated %d byte's addr %.8x\n", nbytes * sizeof(header_t), p + 1);
          return (void*) (p + 1);
        }

      p = p->next;
    }
  while (p != freep);

  return NULL;
}

void
kfree(void *ptr)
{
  header_t *bp = (header_t*) ptr - 1;
  header_t *p = freep;
  dprintf("free addr %.8x\n", p + 1);

  if (freep == NULL)
    {
      freep = bp;
      freep->next = freep;
      return;
    }

  while (!(bp > p && bp < p->next))
    {
      if (p >= p->next && (bp > p || bp < p->next))
        break;

      p = p->next;
    }

  bp->next = p->next;
  p->next = bp;

  if (bp + bp->size == bp->next)
    {
      bp->size += bp->next->size;
      bp->next = bp->next->next;
    }

  if (p + p->size == p->next)
    {
      p->size += p->next->size;
      p->next = p->next->next;
    }
}
