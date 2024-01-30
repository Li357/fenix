#include "socket.h"
#include <stddef.h>
#include <stdio.h>
#include "kernel.h"

tcp_record_t tcp_table[SOCKET_MAX_CONNECTIONS];

int socket_open(uint16_t port) {
  for (size_t i = 0; i < SOCKET_MAX_CONNECTIONS; i++) {
    if (tcp_table[i].state == TCP_CLOSED) {
      tcp_table[i].state      = TCP_OPEN;
      tcp_table[i].local_port = port;
      return i;
    }
  }
  return -1;
}

int socket_listen(int sd) {
  tcp_table[sd].state = TCP_LISTEN;
  return 0;
}

int socket_accept(int sd) {
  if (!tcp_table[sd].event) {
    task_yield();
    return 0;
  }
  puts("New TCP event!\n");
  return 1;
}

int socket_close(int sd) {
  tcp_table[sd].state = TCP_CLOSED;
  return 0;
}

