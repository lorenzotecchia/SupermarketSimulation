void* supervisiona_supermercato(void *arg) {
    sleep(10);  // Attende 10 secondi prima di iniziare

    Supermercato *supermercato = (Supermercato *)arg;
    //stampa la lista di attesa con gli id dei clienti
    printf(COLOR_GREEN "Lista attesa finale di %d Clienti" COLOR_RESET, supermercato->clienti_fuori);
    printf("\n");

    while (1) {
        // Blocca il mutex per leggere e modificare i dati del supermercato
        pthread_mutex_lock(&supermercato->mutex_supermercato);

        // Controlla se possiamo ammettere nuovi clienti
        if (possiamo_ammettere_clienti(supermercato)) {
            int clienti_ammessi = ammetti_clienti(supermercato);
            if(clienti_ammessi){
            printf(COLOR_BLUE "Ammessi %d nuovi clienti. Clienti attualmente presenti: %d.\n" COLOR_RESET, clienti_ammessi, supermercato->clienti_presenti);
            }
            else if (!clienti_ammessi) {
                printf(COLOR_RED "Non ci sono clienti da ammettere o limite massimo raggiunto.\n" COLOR_RESET);
                break;
            }
        } else {
            printf(COLOR_RED "Non ci sono spazi disponibili per nuovi clienti. Clienti attualmente presenti: %d.\n" COLOR_RESET, supermercato->clienti_presenti);
            break;
        }

        // Sblocca il mutex del supermercato
        pthread_mutex_unlock(&supermercato->mutex_supermercato);

        sleep(10);  // Attende 10 secondi prima di ricontrollare
    }

    exit(0);
    return NULL;
}
