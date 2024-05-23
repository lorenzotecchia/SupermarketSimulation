#include "cliente.h"
#include "colors.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

void *createClients(void *arg) {
  for (int i = 0; i <= 10; i++) {
    // Crea un nuovo cliente con parametri casuali
    int oggetti = rand() % 10 + 1; // da 1 a 10 oggetti
    int tempo = rand() % 5 + 1;    // da 1 a 5 secondi di tempo

    Cliente *cliente = newCliente(oggetti, tempo);
    printf("Creato nuovo cliente con %d oggetti, tempo: %d\n", cliente->oggetti,
           cliente->tempo);

    // Connetti al server e invia il cliente
    int client_fd;
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Errore nella creazione del socket");
      exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
      perror("Indirizzo non valido o non supportato");
      exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
        0) {
      perror("Connessione fallita");
      exit(EXIT_FAILURE);
    }

    // Invia il cliente al server
    send(client_fd, &cliente->oggetti, sizeof(int), 0);
    send(client_fd, &cliente->tempo, sizeof(int), 0);

    // Chiudi la connessione e libera la memoria del cliente
    close(client_fd);
    free(cliente);

    // Aspetta un po' prima di creare un altro cliente
    usleep(1);
  }
}

int main() {
  srand(time(NULL)); // Inizializza il generatore di numeri casuali con un seed
                     // diverso ogni volta

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, createClients, NULL);

  pthread_join(thread_id, NULL);

  return 0;
}
