#ifndef _STDLIB_H
#define _STDLIB_H

#include <mem/heap.h>

#define NULL (void*)  0
#define EXIT_SUCCESS  0
#define EXIT_FAILURE  1

#define malloc(n) kmalloc(n)
#define free(ptr) kfree(ptr)

int atoi(const char *s);

#endif /* NOT _STDLIB_H */
