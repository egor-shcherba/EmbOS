#include <system/timer.h>
#include <system/thread.h>
#include <system/sched.h>
#include <sys/pit.h>
#include <debug/qemu.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>

#define __DEBUG_HEADER__ "TIMER"

#define MAX_TIMERS 10

struct timer {
  unsigned int exp_start;
  unsigned int ticks;
  struct thread *owner;
};

static volatile unsigned int _experiod = 0;
static struct timer timers[MAX_TIMERS];

void
timer_init(void)
{
  memset(timers, 0, sizeof(timers));
  qprintf("initialized\n");
}

static inline void
timer_set(struct timer *timer, struct thread *owner, unsigned int mseconds)
{
  timer->owner = owner;
  timer->exp_start = _experiod;
  timer->ticks = mseconds;
}

static inline void
timer_unset(struct timer *timer)
{
  timer->exp_start = 0;
  timer->ticks = 0;
  timer->owner = NULL;
}

static inline void
timer_update(struct timer *timer)
{
  if (timer->owner == NULL)
    return;

  struct thread *owner = timer->owner;

  if (timer->exp_start + timer->ticks < _experiod)
    {
      timer_unset(timer);
      sys_thread_wakeup(owner);
    }
}

void
timer_tick(void)
{
  _experiod += 1000 / HZ;

  for (int i = 0; i < MAX_TIMERS; i++)
    timer_update(&timers[i]);
}

static inline struct timer*
get_timer(void)
{
  for (int i = 0; i < MAX_TIMERS; i++)
    {
      if (timers[i].owner == NULL)
        return &timers[i];
    }

  return NULL;
}

int
sys_settimer(unsigned int mseconds)
{
  struct thread *self = (struct thread*) sys_thread_self();
  struct timer *timer = get_timer();

  if (timer == NULL)
    return -EBUSY;

  timer_set(timer, self, mseconds);

  dprintf("set timer to owner '%s' id %d period %d\n", self->name, self->id, mseconds);
  sys_thread_sleep();

  return 0;
}
