#include "list.h"
#include <stddef.h>

void list_init(list_t *l) {
  l->head   = NULL;
  l->length = 0;
}

void list_insert_end(list_t *l, list_item_t *item) {
  if (l->head == NULL) {
    l->head = item;
  } else {
    list_item_t *curr = l->head;
    while (curr->next) curr = curr->next;
    curr->next = item;
  }
  l->length++;
}
