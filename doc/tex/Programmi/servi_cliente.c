void *servi_cliente(void *arg) {
    ParametriCassa *parametri = (ParametriCassa *)arg;
    Cassa *cassa = (Cassa *)parametri->cassa;
    Supermercato *supermercato = (Supermercato *)parametri->supermercato;

    while (1) {
        srand(time(NULL));
        pthread_mutex_lock(&cassa->mutex_cassa);

        // Aspetta che ci siano clienti in coda
        while (cassa->num_clienti_in_coda == 0) {
            pthread_cond_wait(&cassa->coda_vuota, &cassa->mutex_cassa);
        }

        // Prende il primo cliente in coda (FIFO)
        Cliente * cliente = &(cassa->coda[0]);

        // Sposta gli altri clienti avanti nella coda
        for (int i = 0; i < cassa->num_clienti_in_coda - 1; i++) {
            cassa->coda[i] = cassa->coda[i + 1];
        }
        cassa->num_clienti_in_coda--;

        // Sblocca il mutex per permettere ad altri di aggiungere clienti alla coda
        pthread_mutex_unlock(&cassa->mutex_cassa);

        int tempo_servizio = cassa->tempo_fisso + cliente->numero_di_oggetti * rand() % 3 + 1;
        printf("Cassa %d sta servendo il cliente %d per %d secondi [...] \n", cassa->id, cliente->id, tempo_servizio);

        // Simula il servizio del cliente
        sleep(tempo_servizio);

        // Aggiorna il tempo totale di servizio della cassa
        pthread_mutex_lock(&cassa->mutex_cassa);
        cassa->tempo_totale += tempo_servizio;
        pthread_mutex_unlock(&cassa->mutex_cassa);

        printf("Cassa %d ha terminato di servire il cliente %d.\n", cassa->id, cliente->id);

        //elimina il cliente dalla cassa
        pthread_mutex_lock(&supermercato->mutex_supermercato);
        supermercato->clienti_presenti--;
        printf("Cliente %d servito e uscito dal supermercato\n", cliente->id);
        pthread_mutex_unlock(&supermercato->mutex_supermercato);

    }
    return NULL;

