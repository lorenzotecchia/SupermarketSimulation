#include "../server_include/server_models.h"


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
            perror("Allocazione fallita per pthread_arg_t");
            continue;
        }

        client_address_len = sizeof(pthread_arg->client_address);
        new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
        if (new_socket_fd == -1) {
            perror("Errore di accept");
            free(pthread_arg);
            continue;
        }

        pthread_arg->new_socket_fd = new_socket_fd;
        pthread_arg->supermercato = supermercato;

        if (pthread_create(&client_thread, NULL, client_handler, (void *)pthread_arg) != 0) {
            perror("Errore creazione thread per client");
            free(pthread_arg);
            close(new_socket_fd);
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

    //detached thread
    pthread_detach(pthread_self());

    char buffer[BUFFER_SIZE] = {0};

    while (read(new_socket_fd, buffer, BUFFER_SIZE) > 0) {
        char command[20] = {0};
        sscanf(buffer, "%s", command);

        Cliente cliente;
        cliente.id = new_socket_fd; // utilizzo dell'ID socket per rappresentare univocamente il cliente
        switch (command[0]) {
            case 'E':
                if (strncmp(buffer, "ENTRY_REQUEST", 13) == 0) {
                    int time_to_shop = 0, num_items = 0;
                    if (sscanf(buffer + 14, "%d %d", &time_to_shop, &num_items) != 2) {
                        close(new_socket_fd);
                        return NULL;
                    }
                    cliente.tempo_per_scegliere_oggetti = time_to_shop;
                    cliente.numero_di_oggetti = num_items;

                    pthread_mutex_lock(&supermercato->mutex_supermercato);
                    if (supermercato->clienti_fuori < supermercato->max_clienti) {
                        write(new_socket_fd, "ENTRY_ACCEPTED", strlen("ENTRY_ACCEPTED"));
                        supermercato->lista_attesa[supermercato->clienti_fuori++] = &cliente;
                    } else {
                        write(new_socket_fd, "ENTRY_DENIED", strlen("ENTRY_DENIED"));
                    }
                    pthread_mutex_unlock(&supermercato->mutex_supermercato);
                }
            case 'Q':
            break;
            case 'P':
                if (strncmp(buffer, "PAYMENT_REQUEST", 15) == 0) {
                    printf(CYAN_COLOR "Il cliente %d ha effettuato il pagamento\n" RESET_COLOR, cliente.id);
                }
                break;
            case 'N':
                if (strncmp(buffer, "NO_ITEMS_EXIT_REQUEST", 21) == 0) {
                    printf(RESET_COLOR "Il cliente %d ha deciso di uscire senza acquistare nulla\n" RESET_COLOR, cliente.id);
                }
                break;
            default:
                printf("Comando non riconosciuto: %s\n", buffer);
                break;
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(new_socket_fd);
    pthread_exit(NULL);
    return NULL;
}

// Gestore del segnale SIGINT
void signal_handler(int signal_number) {
    if (signal_number == SIGINT) {
        printf("SIGINT received. Shutting down server.\n");
        exit(0);
    }
}

