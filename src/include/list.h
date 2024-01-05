#pragma once

#include <stdint.h>

typedef struct list_item {
  struct list_item *next;
  struct list_item *prev;
  void *item;
} list_item_t;

typedef struct {
  list_item_t *head;
  uint32_t length;
} list_t;

void list_init(list_t *l);
void list_insert_end(list_t *l, list_item_t *item);
