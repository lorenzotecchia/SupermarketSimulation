#include "../include_client/receive.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void receive_message_from_server(int socket_fd, char *buffer) {
  printf("[DEBUG] Attempting to read from server...\n"); // Debugging log

  ssize_t bytes_read = read(socket_fd, buffer, BUFFER_SIZE);
  if (bytes_read == -1) {
    perror("read");
    close(socket_fd);
    exit(1);
  } else if (bytes_read == 0) {
    printf("[DEBUG] Server closed the connection.\n");
  } else {
    buffer[bytes_read] = '\0'; // Null-terminate to safely print as a string
    printf("[DEBUG] Received message from server: %s\n",
           buffer); // Log received message
  }
}
