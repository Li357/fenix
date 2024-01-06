#pragma once

#include <stdint.h>

struct list;
typedef struct list_item {
  struct list_item *next;
  struct list_item *prev;
  void *item;
  struct list *container;
} list_item_t;

typedef struct list {
  list_item_t *head;
  uint32_t length;
} list_t;

void list_init(list_t *l);
void list_insert_end(list_t *l, list_item_t *item);
void list_remove(list_t *l, list_item_t *item);
list_item_t *list_next(list_t *l);
