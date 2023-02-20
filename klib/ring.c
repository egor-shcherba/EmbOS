#include <mem/heap.h>
#include <klib/ring.h>

void
ring_buffer_init(struct ring_buffer *rb, int capacity)
{
  rb->capacity = capacity;
  rb->buffer = kmalloc(sizeof(char) * capacity);
  rb->tail = 0;
  rb->head = 0;
  rb->size = 0;
}

int
ring_buffer_write(struct ring_buffer *rb, char c)
{
  if (ring_buffer_is_full(rb))
    return -1;

  rb->buffer[rb->tail++ % rb->capacity] = c;
  rb->size++;
  return c;
}

int
ring_buffer_read(struct ring_buffer *rb)
{
  if (ring_buffer_is_empty(rb))
    return -1;

  rb->size--;
  return rb->buffer[rb->head++ % rb->capacity];
}

int
ring_buffer_read_tail(struct ring_buffer *rb)
{
  if (ring_buffer_is_empty(rb))
    return -1;

  rb->size--;
  return rb->buffer[--rb->tail % rb->capacity];
}

void
ring_buffer_flush(struct ring_buffer *rb)
{
  rb->size = 0;
  rb->head = 0;
  rb->tail = 0;
}
