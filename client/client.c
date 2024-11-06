#include "client.h"
#define SERVER_NAME_LEN_MAX 255
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  char server_name[SERVER_NAME_LEN_MAX + 1] = {0};
  int server_port, socket_fd, num_clients = 1;

  get_server_info(argc, argv, server_name, &server_port, &num_clients); 
  print_welcome_message();

  for (int i = 0; i < num_clients; i++) {
    if (fork() == 0) {
      socket_fd = connect_to_server(server_name, server_port);
<<<<<<< HEAD

      int time_to_shop, num_items;
      generate_client_params(&time_to_shop, &num_items);

=======
      printf("Client connesso correttamente\n");
      int time_to_shop, num_items;
      generate_client_params(&time_to_shop, &num_items);
      printf("Cliente generato\n");
      printf("Tempo per gli acquisti [%d]\n", time_to_shop);
      printf("Numero di oggetti [%d]\n", num_items);
      
>>>>>>> refs/remotes/origin/right
      if (request_entry_to_supermarket(socket_fd, time_to_shop, num_items) == 0) {
        shop_for_items(time_to_shop);

        if (num_items > 0) {
          request_queue_to_checkout(socket_fd);
          wait_in_queue_and_pay(socket_fd, num_items);
        } else {
          handle_no_items_exit(socket_fd);
        }
      }

      close(socket_fd);
      exit(0);
    }
  }

  for (int i = 0; i < num_clients; i++) {
    wait(NULL);
  }

  return 0;
}

void generate_client_params(int *time_to_shop, int *num_items) {
  *time_to_shop = rand() % 10 + 1;
  *num_items = rand() % 20;
}

int request_entry_to_supermarket(int socket_fd, int time_to_shop, int num_items) {
  char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, "ENTRY_REQUEST %d %d", time_to_shop, num_items);
  write(socket_fd, buffer, strlen(buffer));

  receive_message_from_server(socket_fd, buffer);
  if (strcmp(buffer, "ENTRY_ACCEPTED") == 0) {
    printf("Richiesta di entrare accettata\n");
    return 0;
  } else {
    printf("Entry denied: %s\n", buffer);
    return -1;
  }
}

void shop_for_items(int time_to_shop) {
<<<<<<< HEAD
  sleep(time_to_shop);
=======
  printf("Il cliente impiegherà [%d] secondi per fare acquisti\n", time_to_shop);
  sleep(time_to_shop);  // Simula il tempo speso a fare acquisti
>>>>>>> refs/remotes/origin/right
}

int request_queue_to_checkout(int socket_fd) {
  char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, "QUEUE_REQUEST");
  write(socket_fd, buffer, strlen(buffer));

  receive_message_from_server(socket_fd, buffer);
  printf("Assigned to queue: %s\n", buffer);
  return 0;
}

void wait_in_queue_and_pay(int socket_fd, int num_items) {
  char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, "PAYMENT_REQUEST %d", num_items);
  write(socket_fd, buffer, strlen(buffer));

  receive_message_from_server(socket_fd, buffer);
  printf("Payment complete: %s\n", buffer);
}

void handle_no_items_exit(int socket_fd) {
  char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, "NO_ITEMS_EXIT_REQUEST");
  write(socket_fd, buffer, strlen(buffer));
  receive_message_from_server(socket_fd, buffer);
}
