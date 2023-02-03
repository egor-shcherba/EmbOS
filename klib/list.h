#ifndef _LIST_H
#define _LIST_H


/* circular, double linked-list */
#define offsetof(type, member) \
  __builtin_offsetof(type, member)

#define container_of(ptr, type, member) \
  (type*) ((char*) ptr - offsetof(type, member))

struct list {
  struct list *next;
  struct list *prev;
};


static inline void
list_init(struct list *list)
{
  list->next = list;
  list->prev = list;
}

static inline int
list_empty(struct list *list)
{
  return list->next == list;
}

/* private method */
static inline void
__list_add(struct list *prev, struct list *next, struct list *node)
{
  next->prev = node;
  node->next = next;

  prev->next = node;
  node->prev = prev;
}

static inline void
list_add_head(struct list *head, struct list *node)
{
  __list_add(head, head->next, node);
}

static inline void
list_add_tail(struct list *head, struct list *node)
{
  __list_add(head->prev, head, node);
}

static inline void
list_remove(struct list *list)
{
  list->prev->next = list->next;
  list->next->prev = list->prev;
}

static inline struct list*
list_first(const struct list *head)
{
  return head->next;
}

static inline struct list*
list_last(const struct list *head)
{
  return head->prev;
}

#define list_entry(list, type, member) container_of(list, type, member)

#define list_first_entry(head, type, member) \
  list_entry(list_first(head), type, member)

#define list_last_entry(head, type, member) \
  list_entry(list_last(head), type, member)

#define list_for_each(pos, head) \
  for ((pos) = list_first(head); (pos) != head; (pos) = (pos)->next)

#endif /* NOT _LIST_H */
