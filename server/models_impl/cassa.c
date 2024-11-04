#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../include/models.h"
#include <unistd.h>

// Funzione per simulare il servizio di un cliente alla cassa
void *servi_cliente(void *arg) {
    Cassa *cassa = (Cassa *)arg;

    while (1) {
        // Blocca il mutex della cassa
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

        // Calcola il tempo di servizio per il cliente
        int tempo_servizio = cassa->tempo_fisso + cliente->numero_di_oggetti * 1; // esempio: 1 secondo per oggetto
        printf("Cassa %d sta servendo il cliente %d per %d secondi.\n", cassa->id, cliente->id, tempo_servizio);

        // Simula il servizio del cliente
        sleep(tempo_servizio);

        // Aggiorna il tempo totale di servizio della cassa
        pthread_mutex_lock(&cassa->mutex_cassa);
        cassa->tempo_totale += tempo_servizio;
        pthread_mutex_unlock(&cassa->mutex_cassa);

        printf("Cassa %d ha terminato di servire il cliente %d.\n", cassa->id, cliente->id);

        // Simula l'uscita del cliente (qui si potrebbe fare il free se i clienti sono allocati dinamicamente)
    }

    return NULL;
}
