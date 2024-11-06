#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "include_server/models.h"

typedef struct pthread_arg_t {
    Supermercato *supermercato;
    int new_socket_fd;
    struct sockaddr_in client_address;
} pthread_arg_t;

void setup_server_socket(int server_port, int *socket_fd);
void accept_connections(int socket_fd, Supermercato *supermercato);
void signal_handler(int signal_number);
void *client_handler(void *arg);

int main(int argc, char *argv[]) {
    int server_port = 0;
    int socket_fd;
    Supermercato supermercato;

    signal(SIGINT, signal_handler);

    server_port = (argc > 1) ? atoi(argv[1]) : 0;
    if (server_port <= 0) {
        printf("Enter Port: ");
        scanf("%d", &server_port);
    }

    int num_casse = (argc > 2) ? atoi(argv[2]) : 0;
    if (num_casse <= 0 && num_casse > MAX_CASHIERS) {
        printf("Enter number of checkout counters: ");
        scanf("%d", &num_casse);
    }

    inizializza_supermercato(&supermercato, num_casse, MAX_CLIENTS, 0);
    setup_server_socket(server_port, &socket_fd);

    pthread_t supermarket_thread;
    if (pthread_create(&supermarket_thread, NULL, supervisiona_supermercato, (void *)&supermercato) != 0) {
        perror("pthread_create");
        exit(1);
    }
    printf("Thread del supermercato creato correttamente\n");

    accept_connections(socket_fd, &supermercato);

    pthread_join(supermarket_thread, NULL);
    close(socket_fd);
    return 0;
}

void setup_server_socket(int server_port, int *socket_fd) {
    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (bind(*socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(1);
    }

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
        pthread_arg = (pthread_arg_t *)malloc(sizeof(pthread_arg_t));
        if (!pthread_arg) {
            perror("malloc");
            continue;
        }

        client_address_len = sizeof(pthread_arg->client_address);
        new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
        if (new_socket_fd == -1) {
            perror("accept");
            free(pthread_arg);
            continue;
        }

        pthread_arg->new_socket_fd = new_socket_fd;
        pthread_arg->supermercato = supermercato;

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
    free(arg);

    char buffer[BUFFER_SIZE] = {0};
    memset(buffer, 0, BUFFER_SIZE);

    // Continua a leggere finché ci sono dati dal client
    while (read(new_socket_fd, buffer, BUFFER_SIZE) > 0) {
        // Verifica se il messaggio ricevuto è una richiesta di ingresso
        if (strncmp(buffer, "ENTRY_REQUEST", 13) == 0) {
            int time_to_shop = 0, num_items = 0;

            // Stampa di debug per verificare il contenuto del buffer ricevuto dal client
            printf("DEBUG: Buffer ricevuto dal client: %s\n", buffer);

            // Parsing dei parametri dal buffer
            if (sscanf(buffer + 14, "%d %d", &time_to_shop, &num_items) != 2) {
                printf("Errore nel parsing dei dati dal buffer: %s\n", buffer);
                close(new_socket_fd);  // Chiude la connessione in caso di errore
                return NULL;
            }

            // Debug per i valori parsati
            printf("DEBUG: time_to_shop = %d, num_items = %d\n", time_to_shop, num_items);

            pthread_mutex_lock(&supermercato->mutex_supermercato);
            if (supermercato->clienti_fuori < supermercato->max_clienti) {
                write(new_socket_fd, "ENTRY_ACCEPTED", strlen("ENTRY_ACCEPTED"));
                printf("Richiesta di ingresso accettata per il cliente.\n");
            } else {
                write(new_socket_fd, "ENTRY_DENIED", strlen("ENTRY_DENIED"));
                printf("Richiesta di ingresso negata per il cliente.\n");
            }
            pthread_mutex_unlock(&supermercato->mutex_supermercato);
        } 
        
        // Gestione della richiesta di coda per la cassa
        else if (strcmp(buffer, "QUEUE_REQUEST") == 0) {
            write(new_socket_fd, "ASSIGNED_TO_QUEUE", strlen("ASSIGNED_TO_QUEUE"));
            printf("Cliente assegnato alla coda.\n");
        } 
        
        // Gestione della richiesta di pagamento
        else if (strncmp(buffer, "PAYMENT_REQUEST", 15) == 0) {
            write(new_socket_fd, "PAYMENT_COMPLETE", strlen("PAYMENT_COMPLETE"));
            printf("Pagamento completato per il cliente.\n");
        } 
        
        // Gestione dell'uscita senza oggetti
        else if (strcmp(buffer, "NO_ITEMS_EXIT_REQUEST") == 0) {
            write(new_socket_fd, "EXIT_CONFIRMED", strlen("EXIT_CONFIRMED"));
            printf("Uscita confermata per il cliente senza oggetti.\n");
        }
    }

    close(new_socket_fd);
    return NULL;
}



void signal_handler(int signal_number) {
    if (signal_number == SIGINT) {
        printf("SIGINT received. Shutting down server.\n");
        exit(0);
    }
}
