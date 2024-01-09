#include <sys/stat.h>
#include <unistd.h>
#include "socket.h"
#include "usart.h"

int _write(int fd, char *ptr, int len) {
  if (fd == STDOUT_FILENO) usart_transmit(USART1, ptr, len);
  return -1;
}

int _read(int fd, char *ptr, int len) {
  return -1;
}

int _close(int fd) {
  if (fd >= SOCKET_FD_BASE) { return socket_close(fd); }
  return -1;
}

int _lseek(int fd, char *ptr, int dir) {
  return 0;
}

int _fstat(int fd, struct stat *st) {
  return -1;
}

int _isatty(int fd) {
  return 1;
}
