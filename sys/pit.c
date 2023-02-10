#include <sys/pit.h>
#include <sys/pic.h>
#include <sys/io.h>
#include <debug/qemu.h>
#include <system/sched.h>
#include <system/timer.h>
#include <stdint.h>

#define PIT_FREQ      1193182

#define PIT_CMD_PORT  0x43
#define PIT_CHANNEL0  0x40

#define __DEBUG_HEADER__  "PIT"

static void
pit_tick(void)
{
  schedule();
  timer_tick();
}

void
pit_init(void)
{
  uint16_t latch = PIT_FREQ / HZ;

  outb(PIT_CMD_PORT, 0x36);
  outb(PIT_CHANNEL0, latch & 0xFF);
  outb(PIT_CHANNEL0, latch >> 8);

  irq_set_handler(0, pit_tick);
}
