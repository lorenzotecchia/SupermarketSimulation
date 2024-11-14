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

