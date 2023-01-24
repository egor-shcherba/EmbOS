#ifndef _HEAP_H
#define _HEAP_H

#include <stddef.h>

void heap_init(void);
void heap_dump(void);

void *kmalloc(size_t nbytes);
void kfree(void *p);

#endif /* NOT _HEAP_H */
