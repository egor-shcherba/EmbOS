#ifndef _RING_H
#define _RING_H

struct ring_buffer {
  int capacity;
  char *buffer;
  int tail;
  int head;
  int size;
};

static inline int
ring_buffer_is_full(struct ring_buffer *rb)
{
  if (rb->size >= rb->capacity)
    return 1;

  return 0;
}

static inline int
ring_buffer_is_empty(struct ring_buffer *rb)
{
  if (rb->head == rb->tail)
    return 1;

  return 0;
}

void ring_buffer_init(struct ring_buffer *rb, int capacity);
int ring_buffer_write(struct ring_buffer *rb, char c);
int ring_buffer_read(struct ring_buffer *rb);
int ring_buffer_read_tail(struct ring_buffer *rb);
void ring_buffer_flush(struct ring_buffer *rb);

#endif /* NOT _RING_H */
