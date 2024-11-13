#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "gui.h"
#include "time.h"

// Per il client
void print_welcome_message();
void get_server_info(int argc, char *argv[], char *server_name, int *server_port, int *num_clients);
int connect_to_server(const char *server_name, int server_port);
void generate_client_params(int *time_to_shop, int *num_items);
int request_entry_to_supermarket(int socket_fd, int time_to_shop, int num_items);
void shop_for_items(int time_to_shop);
int request_queue_to_checkout(int socket_fd);
void wait_in_queue_and_pay(int socket_fd, int num_items);
void close_resources(int socket_fd);
void handle_no_items_exit(int socket_fd);

// Per la connessione
int connect_to_server(const char *server_name, int server_port);
void get_server_info(int argc, char *argv[], char *server_name, int *server_port, int *num_clients);

// Per la ricezione dei messaggi
void receive_message_from_server(int socket_fd, char *buffer);
