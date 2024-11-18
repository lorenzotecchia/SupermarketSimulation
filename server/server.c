#include "server_include/server_models.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
    if (num_casse <= 0 || num_casse > MAX_CASHIERS) {
        printf("Enter number of checkout counters: ");
        scanf("%d", &num_casse);
    }

    // Stampa del messaggio di benvenuto
    print_welcome_message(num_casse, server_port);

    // Inizializza il supermercato con le casse e i parametri   
    initialize_supermarket(&supermercato, num_casse, MAX_CLIENTS);

    // Crea un thread per supervisionare il supermercato
    pthread_t supermarket_thread;
    if (pthread_create(&supermarket_thread, NULL, manage_supermarket, (void *)&supermercato) != 0) {
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

        if (pthread_create(&cassa_thread, NULL, serve_clients, (void *)parametri) != 0) {
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
