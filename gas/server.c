#include "cassa.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_CASSE 15
#define PORT 8080
#define BACKLOG 10

int client_number_in_supermarket = 0;

typedef struct _server {
  Cassa *casse[NUM_CASSE];
  pthread_t threads[NUM_CASSE];
  int server_fd;
} Server;

Server *server; // Definizione della variabile server

void *gestisciCassa(void *arg) {
  Cassa *cassa = (Cassa *)arg;
  while (1) {
    processaClienti(cassa);
  }
  return NULL;
}

Server *creaServer() {
  Server *server = (Server *)malloc(sizeof(Server));
  for (int i = 0; i < NUM_CASSE; i++) {
    Cassiere *cassiere = newCassiere(i);
    server->casse[i] = creaCassa(cassiere);
    pthread_create(&server->threads[i], NULL, gestisciCassa,
                   (void *)server->casse[i]);
  }

  server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server->server_fd == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server->server_fd, (struct sockaddr *)&address, sizeof(address)) <
      0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server->server_fd, BACKLOG) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  return server;
}

/*
 * Liberare prima il client, facendo uscire il cliente dal supermercato e poi
 * chiudere la socket col client
 * */
void *gestisciClient(void *arg) {
  int client_socket = *(int *)arg;
  free(arg);

  int oggetti, tempo;
  read(client_socket, &oggetti, sizeof(int));
  read(client_socket, &tempo, sizeof(int));

  Cliente *cliente = newCliente(oggetti, tempo);
  printf("Ricevuto cliente con %d oggetti, tempo: %d\n", cliente->oggetti,
         cliente->tempo);

  // Trova la cassa con la coda più corta
  Cassa *cassa_scelta = server->casse[0];
  int lunghezza_coda_minima = lunghezzaCoda(cassa_scelta->codaClienti);

  for (int i = 1; i < NUM_CASSE; i++) {
    int lunghezza_coda_attuale = lunghezzaCoda(server->casse[i]->codaClienti);
    if (lunghezza_coda_attuale < lunghezza_coda_minima) {
      cassa_scelta = server->casse[i];
      lunghezza_coda_minima = lunghezza_coda_attuale;
    }
  }

  // Inserisci il cliente nella coda della cassa scelta
  enqueue(cassa_scelta->codaClienti, cliente);

  close(client_socket);
  return NULL;
}

void accettaClient(Server *server) {
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  while (1) {
    int *client_socket = malloc(sizeof(int));
    *client_socket = accept(server->server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&addrlen);
    if (*client_socket < 0) {
      perror("accept");
      free(client_socket);
      continue;
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, gestisciClient, client_socket);
    pthread_detach(thread_id);
  }
}

void liberaServer(Server *server) {
  for (int i = 0; i < NUM_CASSE; i++) {
    liberaCassa(server->casse[i]);
    printf("Simone");
    pthread_cancel(server->threads[i]);
  }
  close(server->server_fd);
  free(server);
}

int main() {
  server = creaServer(); // Assegna il valore alla variabile globale server
  accettaClient(server);
  liberaServer(server);
  return 0;
}
