#pragma once

#include <stdint.h>

typedef struct {
  uint8_t *data;
  uint32_t length;
  uint32_t read_idx;
  uint32_t write_idx;
} ringbuf_t;

void ringbuf_init(ringbuf_t *buf, void *data, uint32_t length);
int ringbuf_write(ringbuf_t *buf, void *src, uint32_t len);
int ringbuf_read(ringbuf_t *buf, void *dst, uint32_t len);
