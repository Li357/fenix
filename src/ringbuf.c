#include "ringbuf.h"
#include <string.h>

// I'm pretty sure we don't need locks if we only have one thread that
// reads and one thread that writes.

void ringbuf_init(ringbuf_t *buf, void *data, uint32_t length) {
  buf->data      = data;
  buf->length    = length;
  buf->read_idx  = 0;
  buf->write_idx = 0;
}

int ringbuf_write(ringbuf_t *buf, void *src, uint32_t len) {
  uint32_t idx = (buf->write_idx + 1) % buf->length;
  if (idx == buf->read_idx) return 0;
  memcpy(&buf->data[buf->write_idx], src, len);
  buf->write_idx = idx;
  return 1;
}

int ringbuf_read(ringbuf_t *buf, void *dst, uint32_t len) {
  if (buf->read_idx == buf->write_idx) return 0;
  memcpy(dst, &buf->data[buf->read_idx], len);
  buf->read_idx = (buf->read_idx + 1) % buf->length;
  return 1;
}
