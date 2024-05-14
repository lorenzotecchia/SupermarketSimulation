#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 3

void *handle_client(void *arg);

typedef struct {
    int id;
    int socket;
} client_info;

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Creazione del socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }

    // Configurazione dell'indirizzo del server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Errore nel binding");
        exit(EXIT_FAILURE);
    }

    // Ascolto delle connessioni in arrivo
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Errore nell'ascolto delle connessioni");
        exit(EXIT_FAILURE);
    }

    printf("Server in ascolto sulla porta %d...\n", PORT);

    int client_counter = 0;
    pthread_t tid[MAX_CLIENTS];
    client_info clients[MAX_CLIENTS];

    while (1) {
        // Accettazione della connessione in arrivo
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) {
            perror("Errore nell'accettare la connessione");
            exit(EXIT_FAILURE);
        }

        printf("Connessione accettata da %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Assegnazione di un ID al client
        clients[client_counter].id = client_counter + 1;
        clients[client_counter].socket = client_socket;

        // Invio dell'ID al client
        sprintf(buffer, "%d", clients[client_counter].id);
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Errore nell'invio dell'ID al client");
            exit(EXIT_FAILURE);
        }

        // Creazione di un thread per gestire il client
        pthread_create(&tid[client_counter], NULL, handle_client, (void *)&clients[client_counter]);

        // Incremento del contatore dei client
        client_counter++;
    }

    // Chiusura del socket del server
    close(server_socket);

    return 0;
}

void *handle_client(void *arg) {
    client_info *client = (client_info *)arg;
    int client_id = client->id;
    int client_socket = client->socket;
    char buffer[BUFFER_SIZE];

    while (1) {
        // Ricezione del messaggio dal client
        if (recv(client_socket, buffer, BUFFER_SIZE, 0) <= 0) {
            printf("Connessione chiusa dal client %d\n", client_id);
            break;
        }

        printf("Messaggio ricevuto dal client %d: %s\n", client_id, buffer);

        // Invio di una conferma al client
        if (send(client_socket, "Messaggio ricevuto dal server", strlen("Messaggio ricevuto dal server"), 0) == -1) {
            perror("Errore nell'invio della conferma al client");
            break;
        }
    }

    // Chiusura della connessione del client
    close(client_socket);
    pthread_exit(NULL);
}
