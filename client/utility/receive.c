#include "../../include/receive.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void receive_message_from_server(int socket_fd, char *buffer) {
  if (read(socket_fd, buffer, BUFFER_SIZE) == -1) {
    perror("read");
    close(socket_fd);
    exit(1);
  }
}
