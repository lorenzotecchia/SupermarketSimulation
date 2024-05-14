#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];
  char quit_message[] = "quit";

  // Creazione del socket
  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Errore nella creazione del socket");
    exit(EXIT_FAILURE);
  }

  // Configurazione dell'indirizzo del server
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  server_addr.sin_port = htons(PORT);

  // Connessione al server
  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("Errore nella connessione al server");
    exit(EXIT_FAILURE);
  }

  printf("Connessione al server riuscita\n");

  // Ricezione dell'ID assegnato dal server
  if (recv(client_socket, buffer, BUFFER_SIZE, 0) == -1) {
    perror("Errore nella ricezione dell'ID dal server");
    exit(EXIT_FAILURE);
  }

  printf("ID assegnato dal server: %s\n", buffer);

  // Ciclo per inviare più messaggi
  while (1) {
    // Invio del messaggio al server
    printf("Inserisci il messaggio da inviare al server (digita 'quit' per "
           "terminare): ");
    fgets(buffer, BUFFER_SIZE, stdin);

    // Rimuovi il newline dal messaggio
    buffer[strcspn(buffer, "\n")] = 0;

    if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
      perror("Errore nell'invio del messaggio");
      exit(EXIT_FAILURE);
    }

    // Controlla se il messaggio è "quit" per terminare la connessione
    if (strcmp(buffer, quit_message) == 0) {
      printf("Terminazione della connessione\n");
      break;
    }
  }

  // Chiusura della connessione
  close(client_socket);

  return 0;
}
