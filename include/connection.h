#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int connect_to_server(const char *server_name, int server_port);
void get_server_info(int argc, char *argv[], char *server_name, int *server_port, int *num_clients);

#endif // CONNECTION_H