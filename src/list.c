#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

void list_init(list_t *l) {
  l->head   = NULL;
  l->length = 0;
}

void list_insert_end(list_t *l, list_item_t *item) {
  assert(l != NULL && item != NULL);
  if (l->head == NULL) {
    assert(l->length == 0);
    item->prev = item;
    item->next = item;
    l->head    = item;
  } else {
    assert(l->length > 0);
    item->prev    = l->head->prev;
    l->head->prev = item;
    item->next    = l->head;
    assert(item->next == l->head);
    assert(item == l->head->prev);
  }
  item->container = l;
  l->length++;
}

void list_remove(list_t *l, list_item_t *item) {
  assert(l != NULL && item != NULL);
  assert(item->prev != NULL && item->next != NULL);
  assert(item->container == l);
  item->prev->next = item->next;
  item->next->prev = item->prev;
  if (item == item->container->head) item->container->head = NULL;
  item->container->length--;
  item->container = NULL;
}

list_item_t *list_next(list_t *l) {
  assert(l != NULL);
  if (l->head == NULL) return NULL;
  l->head = l->head->next;
  assert(l->head != NULL);
  return l->head;
}

