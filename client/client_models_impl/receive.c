#include "../client_include/client_models.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void receive_message_from_server(int socket_fd, char *buffer) {
  ssize_t bytes_read = read(socket_fd, buffer, BUFFER_SIZE);
  if (bytes_read == -1) {
    perror("read");
    close(socket_fd);
    exit(1);
  } else if (bytes_read == 0) {
  } else {
    buffer[bytes_read] = '\0'; //Termina con null per stampare in modo sicuro come una stringa
  }
}
