#include "supermercato.h"
#include "cassa.h"
#include "cliente.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//__________________________________________________________________________________________________//
void inizializza_supermercato(Supermercato *supermercato, int num_casse, int max_clienti, int E) {
    supermercato->num_casse = num_casse;
    supermercato->max_clienti = max_clienti;
    supermercato->clienti_presenti = 0;
    supermercato->clienti_fuori = 0;
    supermercato->E = E;
    pthread_mutex_init(&supermercato->mutex_supermercato, NULL);
    pthread_cond_init(&supermercato->spazio_disponibile, NULL);

    for (int i = 0; i < num_casse; i++) {
        supermercato->casse[i] = (Cassa *)malloc(sizeof(Cassa));
        supermercato->casse[i]->id = i;
        supermercato->casse[i]->tempo_fisso = 1;  // Imposta valori appropriati per ogni cassa
        supermercato->casse[i]->tempo_totale = 0;
        supermercato->casse[i]->num_clienti_in_coda = 0;
        pthread_mutex_init(&supermercato->casse[i]->mutex_cassa, NULL);
        pthread_cond_init(&supermercato->casse[i]->coda_vuota, NULL);
    }
}
//__________________________________________________________________________________________________//

// Funzione che sceglie la cassa con il minor numero di clienti e mette in fila il cliente
void scegli_cassa_per_cliente(Supermercato *supermercato, Cliente *cliente) {
    // Simula il cliente che sceglie gli oggetti
    scegli_oggetti(cliente);

    // Trova la cassa con il minor numero di clienti
    int cassa_min_index = trova_cassa_minima(supermercato);

    // Se è stata trovata una cassa disponibile, mette in fila il cliente in quella cassa
    if (cassa_min_index != -1) {
        Cassa *cassa_scelta = supermercato->casse[cassa_min_index];
        metti_in_fila(cassa_scelta, cliente);  // Usa metti_in_fila per inserire il cliente
        printf("Cliente %d assegnato alla cassa %d.\n", cliente->id, cassa_scelta->id);
    } else {
        printf("Non ci sono casse disponibili per il cliente %d.\n", cliente->id);
    }
}

int trova_cassa_minima(Supermercato *supermercato) {
    int cassa_min_clienti = -1;
    int num_min_clienti = MAX_CLIENTS + 1;

    // Blocca il mutex del supermercato per garantire accesso esclusivo
    pthread_mutex_lock(&supermercato->mutex_supermercato);

    // Cerca la cassa con meno clienti in coda
    for (int i = 0; i < supermercato->num_casse; i++) {
        Cassa *cassa = supermercato->casse[i];

        // Blocca il mutex della cassa corrente
        pthread_mutex_lock(&cassa->mutex_cassa);

        // Controlla se questa cassa ha meno clienti in coda
        if (cassa->num_clienti_in_coda < num_min_clienti) {
            num_min_clienti = cassa->num_clienti_in_coda;
            cassa_min_clienti = i;
        }

        // Sblocca il mutex della cassa corrente
        pthread_mutex_unlock(&cassa->mutex_cassa);
    }

    pthread_mutex_unlock(&supermercato->mutex_supermercato);
    return cassa_min_clienti;  // Restituisce l'indice della cassa con meno clienti
}

//__________________________________________________________________________________________________//

// Funzione per supervisionare il supermercato e ammettere nuovi clienti
void* supervisiona_supermercato(void *arg) {
    Supermercato *supermercato = (Supermercato *)arg;

    while (1) {
        // Blocca il mutex per leggere e modificare i dati del supermercato
        pthread_mutex_lock(&supermercato->mutex_supermercato);

        // Controlla se possiamo ammettere nuovi clienti
        if (possiamo_ammettere_clienti(supermercato)) {
            int clienti_ammessi = ammetti_clienti(supermercato);
            printf("Ammessi %d nuovi clienti. Clienti attualmente presenti: %d.\n", clienti_ammessi, supermercato->clienti_presenti);
        } else {
            printf("Non ci sono clienti da ammettere o limite massimo raggiunto.\n");
        }

        // Sblocca il mutex del supermercato
        pthread_mutex_unlock(&supermercato->mutex_supermercato);

        sleep(10);  // Attende 10 secondi prima di ricontrollare
    }

    return NULL;
}

// Controlla se possiamo ammettere nuovi clienti
int possiamo_ammettere_clienti(Supermercato *supermercato) {
    return supermercato->clienti_presenti <= supermercato->max_clienti - supermercato->E;
}

// Ammette i clienti dalla lista di attesa
int ammetti_clienti(Supermercato *supermercato) {
    int clienti_da_ammettere = supermercato->E;
    int clienti_ammessi = 0;

    // Controlla quanti clienti ci sono effettivamente in lista di attesa
    for (int i = 0; i < clienti_da_ammettere && supermercato->lista_attesa[i] != NULL; i++) {
        Cliente *cliente = supermercato->lista_attesa[i];

        // Aggiungi il cliente al supermercato
        supermercato->clienti_presenti++;
        clienti_ammessi++;

        // Sposta il cliente dalla lista di attesa 
        scegli_cassa_per_cliente(supermercato, cliente);

        // Rimuove il cliente dalla lista di attesa
        supermercato->lista_attesa[i] = NULL;
    }

    // Sposta avanti i clienti rimasti in attesa per mantenere l'ordine FIFO
    sposta_clienti_avanti(supermercato, clienti_da_ammettere);
    return clienti_ammessi;
}

// Sposta avanti i clienti rimanenti nella lista di attesa
void sposta_clienti_avanti(Supermercato *supermercato, int clienti_da_ammettere) {
    for (int i = 0; i < MAX_CLIENTS - clienti_da_ammettere; i++) {
        supermercato->lista_attesa[i] = supermercato->lista_attesa[i + clienti_da_ammettere];
    }
}
//__________________________________________________________________________________________________//
//__________________________________________________________________________________________________//