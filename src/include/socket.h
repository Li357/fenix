#pragma once

#include <stdbool.h>
#include <stdint.h>

#define SOCKET_MAX_CONNECTIONS (128)

typedef enum {
  TCP_CLOSED,
  TCP_OPEN,
  TCP_LISTEN,
  TCP_SYN_RECEIVED,
  TCP_ESTABLISHED,
} tcp_state_t;

typedef struct {
  tcp_state_t state;
  uint32_t snd_next;
  uint32_t snd_unack;
  uint32_t rcv_next;
  uint16_t remote_port;
  uint16_t local_port;
  bool event;
} tcp_record_t;

extern tcp_record_t tcp_table[];

int socket_open(uint16_t port);
int socket_listen(int sd);
int socket_accept(int sd);
int socket_close(int sd);
