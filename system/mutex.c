#include <stdbool.h>
#include <errno.h>
#include <mem/heap.h>
#include <klib/list.h>
#include <stddef.h>
#include <system/mutex.h>
#include <thread.h>
#include <system/thread.h>
#include <debug/qemu.h>
#include <sys/x86.h>

#define __DEBUG_HEADER__  "MUTEX"

struct thread_mutex {
  bool locked;
  struct thread *owner;
  struct list waiters;
};

int
thread_mutex_init(struct thread_mutex **mutex)
{
  struct thread_mutex *m = *mutex;

  if (m != NULL)
    return -EAGAIN;

  m = kmalloc(sizeof(struct thread_mutex));

  if (m == NULL)
    return -ENOMEM;

  m->locked = false;
  m->owner = NULL;
  list_init(&m->waiters);

  *mutex = m;

  dprintf("initialize mutex %.8x\n", m);
  return 0;
}

int
thread_mutex_destroy(struct thread_mutex **mutex)
{
  struct thread_mutex *m = *mutex;

  if (m->owner != NULL)
    return -EBUSY;

  dprintf("destroyed mutex %.8x\n", *mutex);
  kfree(m);
  *mutex = NULL;

  return 0;
}

int
thread_mutex_lock(struct thread_mutex **mutex)
{
  cpu_disable_interrupt();
  struct thread_mutex *m = *mutex;
  struct thread *self = thread_self();

  if (m->locked == true)
    {
      dprintf("mutex %.8x add waiter '%s' id %d\n", m, self->name, self->id); 
      list_add_tail(&m->waiters, &self->node);
      thread_sleep();
    }

  m->locked = true;
  m->owner = self;

  dprintf("mutex %.8x locked by '%s' id %d\n", m, self->name, thread_id());

  cpu_enable_interrupt();

  return 0;
}

int
thread_mutex_trylock(struct thread_mutex **mutex)
{
  struct thread_mutex *m = *mutex;
  struct thread *self = thread_self();

  if (m->owner != NULL)
    return -EBUSY;

  cpu_disable_interrupt();
  m->locked = true;
  m->owner = self;
  cpu_enable_interrupt();

  dprintf("mutex %.8x locked by '%s' id %d\n", m, self->name, thread_id());

  return 0;
}

int
thread_mutex_unlock(struct thread_mutex **mutex)
{
  cpu_disable_interrupt();

  struct thread_mutex *m = *mutex;

  if (m->owner != thread_self())
    return -EPERM;

  dprintf(" mutex %.8x unlocked by '%s' id %d\n",
    m, thread_self()->name, thread_id());

  struct thread *waiter = NULL;

  if (!list_empty(&m->waiters))
    {
      waiter = list_first_entry(&m->waiters, struct thread, node);

      dprintf("mutex %.8x wakeup waiter '%s' id %d\n",
         m, waiter->name, waiter->id);

      list_remove(&waiter->node);
      thread_wakeup(waiter);
    }

  m->locked = false;
  m->owner = NULL;
  
  cpu_enable_interrupt();
  return 0;
}
