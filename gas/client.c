#include "cliente.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

void inviaCliente(int client_fd, Cliente *cliente) {
  send(client_fd, &cliente->oggetti, sizeof(int), 0);
  send(client_fd, &cliente->tempo, sizeof(int), 0);
}

int main() {
  int client_fd;
  struct sockaddr_in serv_addr;

  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Errore nella creazione del socket \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\n Indirizzo non valido o non supportato \n");
    return -1;
  }

  if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    printf("\n Connessione fallita \n");
    return -1;
  }

  Cliente *cliente = newCliente(
      5, 2); // esempio di cliente con 5 oggetti e 2 secondi per oggetto
  printCliente(cliente);
  inviaCliente(client_fd, cliente);

  close(client_fd);
  free(cliente);
  return 0;
}
