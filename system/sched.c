#include <system/sched.h>
#include <debug/qemu.h>
#include <mem/heap.h>
#include <sys/x86.h>
#include <klib/list.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <macros.h>
#include <stdio.h>

#include <sys/pic.h>

#define __DEBUG_HEADER__  "SCHEDULER"

extern __attribute__((noreturn)) void intr_leave(void);

enum THREAD_STATE {
  UNUSED,
  RUNNING,
  SLEEP,
  INTERUPTIBLE,
  DEAD
};

#define THREAD_NAME_LENGTH  64
#define THREAD_STACK_SIZE   0x4000  /* 16 Kib */

struct thread {
  int id;
  char name[THREAD_NAME_LENGTH];
  void *stackp;
  enum THREAD_STATE state;
  struct list node;
};

struct runq_queue {
  struct thread *current;
  struct list *ready_queue;
  struct thread *idle;
};

#define MAX_THREAD  16

static struct thread thread_table[MAX_THREAD] = { 0 };
static int id = -1;
static struct runq_queue *runq;

void*
thread_idle(void *arg)
{
  UNUSED(arg);

  for (;;)
    printf("IDLE\n");

  return NULL;
}

void
thread_main(void *(*start_routine)(void *arg), void *arg)
{
  UNUSED(start_routine);
  UNUSED(arg);

  start_routine(arg);

  cpu_disable_interrupt();
  cpu_halt();
}

static void
thread_set_name(struct thread *thread, const char *name)
{
  strcpy(thread->name, name);
}

static void
thread_set_state(struct thread *thread, enum THREAD_STATE state)
{
  thread->state = state;
}

static struct thread*
alloc_thread(void)
{
  for (int i = 0; i < MAX_THREAD; i++)
    {
      if (thread_table[i].state == UNUSED)
        return &thread_table[i];
    }

  return NULL;
}

static void
sched_runq_enqueue(struct thread *thread)
{
  list_add_tail(runq->ready_queue, &thread->node);
}

static struct thread*
sched_runq_dequeue(void)
{
  struct thread *thread = NULL;

  if (list_empty(runq->ready_queue))
    return thread;

  thread = list_first_entry(runq->ready_queue, struct thread, node);
  list_remove(&thread->node);

  return thread;
}

static struct thread*
sched_runq_get_current(void)
{
  return runq->current;
}

static struct thread*
sched_runq_get_next(void)
{
  struct thread *next = sched_runq_dequeue();
  struct thread *current =runq->current;

  if (next == NULL
      && current != NULL
      && current != runq->idle
      && current->state == RUNNING)
    {
      return runq->current;
    }

  if (next == NULL)
    return runq->idle;

  return next;
}

void
schedule(void)
{
  struct thread *prev, *next;

  prev = sched_runq_get_current();
  next = sched_runq_get_next();

  if (prev != runq->idle && prev->state == RUNNING)
    sched_runq_enqueue(prev);

  runq->current = next;
}

int
sys_thread_yield(void)
{
  schedule();
  return 0;
}

int
sys_thread_sleep(void)
{
  struct thread *thread = sched_runq_get_current();
  thread_set_state(thread, SLEEP);

  dprintf("thread sleep '%s'\n", thread->name);
  schedule();


  return 0;
}

int
sys_thread_wakeup(struct thread *thread)
{
  thread_set_state(thread, RUNNING);
  list_add_tail(runq->ready_queue, &thread->node);

  dprintf("thread wakeup '%s'\n", thread->name);

  return 0;
}

static inline void
stack_init(
  void **stackp,
  size_t stack_size,
  void *(**start_routine)(void *arg),
  void *arg
  )
{
  uint32_t *stack = *stackp;
  stack = (uint32_t*) ((uint8_t*) stack + stack_size);

  *--stack = (uint32_t) start_routine; /* arg 2*/
  *--stack = (uint32_t) arg;           /* arg 1*/

  *--stack = 0x202;   /* eflags */
  *--stack = 0x8;     /* cs */
  *--stack = (uint32_t) thread_main; /* eip */

  *--stack = 0x0;
  *--stack = 0x0;

  *--stack = 0x0;     /* eax */
  *--stack = 0x0;     /* ebx */
  *--stack = 0x0;     /* ecx */
  *--stack = 0x0;     /* edx */
  *--stack = 0x0;     /* ebp */
  *--stack = 0x0;     /* esi */
  *--stack = 0x0;     /* edi */

  *stackp = stack;
}

int sys_thread_create(
  struct thread **thread,
  const char *name,
  void *(**start_routine)(void *arg),
  void *arg)
{
  struct thread *new_thread = alloc_thread();

  if (new_thread == NULL)
    return -EAGAIN;

  void *stack = kmalloc(THREAD_STACK_SIZE);

  if (stack == NULL)
    {
      new_thread->state = UNUSED;
      return -ENOMEM;
    }

  stack_init(&stack, THREAD_STACK_SIZE, start_routine, arg);
  thread_set_name(new_thread, name);
  thread_set_state(new_thread, RUNNING);
  new_thread->id = ++id;
  new_thread->stackp = stack;
  *thread = new_thread;

  sched_runq_enqueue(new_thread);
  dprintf("create thread '%s' id %d\n", new_thread->name, new_thread->id);

  return 0;
}

void
sched_save_current_context(void *esp)
{
  runq->current->stackp = esp;
}


void*
sched_get_current_context(void)
{
  static int prev_id = -1;
  struct thread *current = runq->current;

  if (prev_id != current->id)
    {
      prev_id = current->id;
      dprintf("switch to '%s' id %d\n", current->name, current->id);
    }

  return current->stackp;
}

static inline unsigned int
__fn_obj_conv(void *(fn)(void*))
{
  unsigned int addr = (unsigned int) fn;
  return addr;
}

static void
thread_init_idle(void)
{
  struct thread *thread = &thread_table[0];
  thread->stackp = kmalloc(THREAD_STACK_SIZE);
  stack_init(&thread->stackp, THREAD_STACK_SIZE, (void*)__fn_obj_conv(&thread_idle), NULL);
  thread_set_name(thread, "IDLE");
  thread_set_state(thread, RUNNING);
  thread->id = ++id;
}

void
sched_init(void)
{
  for (int i = 0; i < MAX_THREAD; i++)
    memset(&thread_table[i], 0, sizeof(struct thread));

  thread_init_idle();
  runq = kmalloc(sizeof(*runq));

  runq->current = NULL;
  list_init(runq->ready_queue);
  runq->idle = &thread_table[0];

  runq->current = NULL;

  irq_set_handler(0, schedule);

  dprintf("initialized\n");
}

__attribute__((noreturn)) void
sched_enable(void)
{
  schedule();
  intr_leave();
}
