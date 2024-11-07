#include "include_server/server.h"

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

    int num_casse = (argc > 2) ? atoi(argv[3]) : 0;
    if (num_casse <= 0 && num_casse > MAX_CASHIERS) {
        printf("Enter number of checkout counters: ");
        scanf("%d", &num_casse);
    }

    // Inizializza il supermercato con le casse e i parametri
    inizializza_supermercato(&supermercato, num_casse, MAX_CLIENTS);

    // Crea un thread per supervisionare il supermercato
    pthread_t supermarket_thread;
    if (pthread_create(&supermarket_thread, NULL, supervisiona_supermercato, (void *)&supermercato) != 0) {
        perror("pthread_create");
        exit(1);
    }

    // Crea un thread per ogni cassa per servire i clienti
    for (int i = 0; i < supermercato.num_casse; i++) {
        pthread_t cassa_thread;
        ParametriCassa *parametri = malloc(sizeof(ParametriCassa));
        if (parametri == NULL) {
            perror("malloc");
            exit(1);
        }
        parametri->cassa = (void *)supermercato.casse[i];
        parametri->supermercato = (void *)&supermercato;

        if (pthread_create(&cassa_thread, NULL, servi_cliente, (void *)parametri) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    setup_server_socket(server_port, &socket_fd);
    accept_connections(socket_fd, &supermercato);

    pthread_join(supermarket_thread, NULL);
    close(socket_fd);
    return 0;
}

// Configura il server socket per ascoltare le connessioni in entrata
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

// Accetta connessioni e crea un thread per ogni cliente
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

// Gestione della connessione con il client
void *client_handler(void *arg) {
    pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
    int new_socket_fd = pthread_arg->new_socket_fd;
    Supermercato *supermercato = pthread_arg->supermercato;
    free(arg);

    char buffer[BUFFER_SIZE] = {0};

    while (read(new_socket_fd, buffer, BUFFER_SIZE) > 0) {
        char command[20] = {0};
        sscanf(buffer, "%s", command);

        switch (command[0]) {
            case 'E': // ENTRY_REQUEST --> richiesta di entrata al supermercato, viene aggiunto nella lista di attesa fuori il supermercato.
                if (strncmp(buffer, "ENTRY_REQUEST", 13) == 0) {
                    int time_to_shop = 0, num_items = 0;
                    if (sscanf(buffer + 14, "%d %d", &time_to_shop, &num_items) != 2) {
                        close(new_socket_fd);
                        return NULL;
                    }

                    Cliente cliente;
                    cliente.id = new_socket_fd; // utilizzo dell'ID socket per rappresentare univocamente il cliente
                    cliente.tempo_per_scegliere_oggetti = time_to_shop;
                    cliente.numero_di_oggetti = num_items;

                    pthread_mutex_lock(&supermercato->mutex_supermercato);
                    if (supermercato->clienti_fuori < supermercato->max_clienti) {
                        write(new_socket_fd, "ENTRY_ACCEPTED", strlen("ENTRY_ACCEPTED"));
                        supermercato->lista_attesa[supermercato->clienti_fuori++] = &cliente;
                        printf("cliente con id : %d in lista attesa.\n", cliente.id);
                    } else {
                        write(new_socket_fd, "ENTRY_DENIED", strlen("ENTRY_DENIED"));
                    }
                    pthread_mutex_unlock(&supermercato->mutex_supermercato);
                }
                break;

            // Altri casi di gestione delle richieste
            default:
                //printf("Comando non riconosciuto: %s\n", buffer);
                break;
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(new_socket_fd);
    return NULL;
}

// Gestore del segnale SIGINT
void signal_handler(int signal_number) {
    if (signal_number == SIGINT) {
        printf("SIGINT received. Shutting down server.\n");
        exit(0);
    }
}
