#ifndef _MUTEX_H
#define _MUTEX_H

#include <stdbool.h>
#include <klib/list.h>

struct thread_mutex;

int thread_mutex_init(struct thread_mutex **mutex);
int thread_mutex_destroy(struct thread_mutex **mutex);

int thread_mutex_lock(struct thread_mutex **mutex);
int thread_mutex_trylock(struct thread_mutex **mutex);

int thread_mutex_unlock(struct thread_mutex **mutex);

#endif /* NOT _MUTEX_H */
