#include "socket.h"
#include <stddef.h>

#define idx(x) ((x)-SOCKET_FD_BASE)

tcp_record_t tcp_table[SOCKET_MAX_CONNECTIONS];

int socket_init(uint16_t port) {
  for (size_t i = 0; i < SOCKET_MAX_CONNECTIONS; i++) {
    if (tcp_table[i].state == TCP_CLOSED) {
      tcp_table[i].state      = TCP_OPEN;
      tcp_table[i].local_port = port;
      return i + SOCKET_FD_BASE;
    }
  }
  return -1;
}

int socket_listen(int fd) {
  tcp_table[idx(fd)].state = TCP_LISTEN;
  return 0;
}

int socket_close(int fd) {
  tcp_table[idx(fd)].state = TCP_CLOSED;
  return 0;
}

