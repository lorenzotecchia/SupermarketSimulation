#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_NAME_LEN_MAX 256

int connect_to_server(const char *server_name, int server_port) {
  int socket_fd;
  struct hostent *server_host;
  struct sockaddr_in server_address;

  /* Get server host from server name. */
  server_host = gethostbyname(server_name);
  if (!server_host) {
    fprintf(stderr, "Could not resolve host\n");
    exit(1);
  }

  /* Initialise server address */
  memset(&server_address, 0, sizeof server_address);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(server_port);
  memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

  /* Create TCP socket */
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  /* Connect to server */
  if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address) == -1) {
    perror("connect");
    close(socket_fd);
    exit(1);
  }

  return socket_fd;
}

void get_server_info(int argc, char *argv[], char *server_name, int *server_port, int *num_clients) {
  if (argc > 1) {
    strncpy(server_name, argv[1], SERVER_NAME_LEN_MAX);
  } else {
    fprintf(stderr, "Usage: %s <server_name> <server_port> <num_clients>\n", argv[0]);
    exit(1);
  }

  if (argc > 2) {
    *server_port = atoi(argv[2]);
  } else {
    fprintf(stderr, "Usage: %s <server_name> <server_port> <num_clients>\n", argv[0]);
    exit(1);
  }

  if (argc > 3) {
    *num_clients = atoi(argv[3]);
  } else {
    fprintf(stderr, "Usage: %s <server_name> <server_port> <num_clients>\n", argv[0]);
    exit(1);
  }
}