#include "../include_client/send.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void send_interval_to_server(int socket_fd) {
  char buffer[BUFFER_SIZE];
  int min, max;

  printf("Enter minimum number: ");
  scanf("%d", &min);
  printf("Enter maximum number: ");
  scanf("%d", &max);

  sprintf(buffer, "%d %d", min, max);
  if (write(socket_fd, buffer, strlen(buffer)) == -1) {
    perror("write");
    close(socket_fd);
    exit(1);
  }
}
