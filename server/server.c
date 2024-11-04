#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "../include/models.h"


// Include il tuo header per le funzioni del supermercato

typedef struct pthread_arg_t {
    Supermercato *supermercato; // Riferimento al supermercato
    int new_socket_fd; // File descriptor del socket
    struct sockaddr_in client_address; // Indirizzo del client
} pthread_arg_t;

void setup_server_socket(int server_port, int *socket_fd);
void accept_connections(int socket_fd, Supermercato *supermercato);
void signal_handler(int signal_number);
void *client_handler(void *arg);

int main(int argc, char *argv[]) {
    int server_port = 0;
    int socket_fd;
    Supermercato supermercato;

    // Inizializza il gestore di segnali per una chiusura sicura
    signal(SIGINT, signal_handler);

    // Ottieni la porta del server dagli argomenti della riga di comando o dall'input
    server_port = (argc > 1) ? atoi(argv[1]) : 0;
    if (server_port <= 0) {
        printf("Enter Port: ");
        scanf("%d", &server_port);
    }

    // Ottieni il numero di casse
    int num_casse = (argc > 2) ? atoi(argv[2]) : 0;
    if (num_casse <= 0 && num_casse > MAX_CASHIERS) {
        printf("Enter number of checkout counters: ");
        scanf("%d", &num_casse);
    }

    // Inizializza il supermercato
    inizializza_supermercato(&supermercato, num_casse, MAX_CLIENTS, 5);

    // Crea e configura il socket del server
    setup_server_socket(server_port, &socket_fd);

    // Avvia il thread di supervisione del supermercato
    pthread_t supermarket_thread;
    if (pthread_create(&supermarket_thread, NULL, supervisiona_supermercato, (void *)&supermercato) != 0) {
        perror("pthread_create");
        exit(1);
    }

    // Accetta le connessioni in un ciclo
    accept_connections(socket_fd, &supermercato);

    // Pulizia delle risorse
    pthread_join(supermarket_thread, NULL);
    close(socket_fd);
    return 0;
}

void setup_server_socket(int server_port, int *socket_fd) {
    struct sockaddr_in server_address;

    // Inizializza la struttura dell'indirizzo del server
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Crea il socket TCP
    if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Esegui il bind al socket
    if (bind(*socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(1);
    }

    // Inizia ad ascoltare sul socket
    if (listen(*socket_fd, 5) == -1) {
        perror("listen");
        exit(1);
    }
}

void accept_connections(int socket_fd, Supermercato *supermercato) {
    pthread_t client_thread;
    pthread_arg_t *pthread_arg;
    socklen_t client_address_len;
    int new_socket_fd;

    while (1) {
        // Alloca memoria per l'argomento del thread
        pthread_arg = (pthread_arg_t *)malloc(sizeof(pthread_arg_t));
        if (!pthread_arg) {
            perror("malloc");
            continue;
        }

        // Accetta la connessione del client
        client_address_len = sizeof(pthread_arg->client_address);
        new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
        if (new_socket_fd == -1) {
            perror("accept");
            free(pthread_arg);
            continue;
        }

        // Inizializza l'argomento del pthread
        pthread_arg->new_socket_fd = new_socket_fd;
        pthread_arg->supermercato = supermercato;

        // Crea un thread per gestire la connessione del client
        if (pthread_create(&client_thread, NULL, client_handler, (void *)pthread_arg) != 0) {
            perror("pthread_create");
            free(pthread_arg);
            continue;
        }
    }
}

void *client_handler(void *arg) {
    pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
    int new_socket_fd = pthread_arg->new_socket_fd;
    Supermercato *supermercato = pthread_arg->supermercato;
    free(arg); // Libera la memoria allocata per l'argomento

    char buffer[BUFFER_SIZE] = {0};
    Cliente *cliente = malloc(sizeof(Cliente)); // Alloca memoria per il Cliente

    // Ricevi l'ID del cliente, numero di oggetti e tempo per scegliere
    if (read(new_socket_fd, buffer, BUFFER_SIZE) == -1) {
        perror("read");
        close(new_socket_fd);
        free(cliente);
        return NULL;
    }

    sscanf(buffer, "%d %d %d", &cliente->id, &cliente->numero_di_oggetti, &cliente->tempo_per_scegliere_oggetti);
    printf("Client %d has %d items and will take %d seconds per item.\n", cliente->id, cliente->numero_di_oggetti, cliente->tempo_per_scegliere_oggetti);

    // Aggiungi il cliente alla lista d'attesa del supermercato
    pthread_mutex_lock(&supermercato->mutex_supermercato);
    if (supermercato->clienti_fuori < supermercato->max_clienti) {
        supermercato->lista_attesa[supermercato->clienti_fuori] = cliente;
        supermercato->clienti_fuori++;
        printf("Client %d added to the waiting list.\n", cliente->id);
        pthread_cond_signal(&supermercato->spazio_disponibile);
    } else {
        printf("Client %d rejected, max clients reached.\n", cliente->id);
       
    }
    return NULL;
}
