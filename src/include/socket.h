#pragma once

#include <stdint.h>

#define SOCKET_MAX_CONNECTIONS (128)

#define SOCKET_FD_BASE         (65536)

typedef enum {
  TCP_CLOSED,
  TCP_OPEN,
  TCP_LISTEN,
  TCP_SYN_RECEIVED,
  TCP_ESTABLISHED,
} tcp_state_t;

typedef struct {
  uint32_t snd_next;
  uint32_t snd_unack;
  uint32_t rcv_next;
  tcp_state_t state;
  uint16_t remote_port;
  uint16_t local_port;
} tcp_record_t;

extern tcp_record_t tcp_table[];

int socket_init(uint16_t port);
int socket_listen(int fd);
int socket_close(int fd);
