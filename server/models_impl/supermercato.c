#include "../include_server/models.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include_server/colors.h"

//__________________________________________________________________________________________________//
void inizializza_supermercato(Supermercato *supermercato, int num_casse, int max_clienti) {
    supermercato->num_casse = num_casse;
    supermercato->max_clienti = max_clienti;
    supermercato->clienti_presenti = 0;
    supermercato->clienti_fuori = 0;
    pthread_mutex_init(&supermercato->mutex_supermercato, NULL);
    pthread_cond_init(&supermercato->spazio_disponibile, NULL);

    for (int i = 0; i < num_casse; i++) {
        supermercato->casse[i] = (Cassa *)malloc(sizeof(Cassa));
        if(supermercato->casse[i] == NULL) {
            perror("malloc");
            exit(1);
        }
        supermercato->casse[i]->id = i;
        supermercato->casse[i]->tempo_fisso = rand () % 3 + 1;
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
        printf(COLOR_GREEN "Cliente %d assegnato alla cassa %d.\n" COLOR_RESET, cliente->id, cassa_scelta->id);
    } else {
        printf(COLOR_RED "Non ci sono casse disponibili per il cliente %d.\n" COLOR_RESET, cliente->id);
    }
}

int trova_cassa_minima(Supermercato *supermercato) {
    int cassa_min_clienti = -1;
    int num_min_clienti = MAX_CLIENTS + 1;
    int num_clienti_in_coda[supermercato->num_casse];

    // Copia temporanea dei dati per evitare di bloccare mutex in ogni iterazione
    for (int i = 0; i < supermercato->num_casse; i++) {
        pthread_mutex_lock(&supermercato->casse[i]->mutex_cassa);
        num_clienti_in_coda[i] = supermercato->casse[i]->num_clienti_in_coda;
        pthread_mutex_unlock(&supermercato->casse[i]->mutex_cassa);
    }

    // Trova la cassa con meno clienti utilizzando i dati copiati
    for (int i = 0; i < supermercato->num_casse; i++) {
        printf(COLOR_YELLOW "Controllo cassa %d con %d clienti in coda\n" COLOR_RESET, i, num_clienti_in_coda[i]);
        
        if (num_clienti_in_coda[i] < num_min_clienti) {
            num_min_clienti = num_clienti_in_coda[i];
            cassa_min_clienti = i;
            if(num_min_clienti==0){
                printf(COLOR_MAGENTA "La cassa è vuota, la assegno\n" COLOR_RESET);
                break;
            }
            printf(COLOR_BLUE "Nuova cassa minima scelta: %d con %d clienti\n" COLOR_RESET, i, num_clienti_in_coda[i]);
        }
    }

    return cassa_min_clienti;
}


//__________________________________________________________________________________________________//

// Funzione per supervisionare il supermercato e ammettere nuovi clienti
void* supervisiona_supermercato(void *arg) {
    sleep(10);  // Attende 10 secondi prima di iniziare

    Supermercato *supermercato = (Supermercato *)arg;
    //stampa la lista di attesa con gli id dei clienti
    printf(COLOR_GREEN "Lista attesa finale: " COLOR_RESET);
    for (int i = 0; i < supermercato->clienti_fuori; i++) {
        printf("%d ", supermercato->lista_attesa[i]->id);
    }
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
        } else {
            printf(COLOR_RED "Non ci sono clienti da ammettere o limite massimo raggiunto.\n" COLOR_RESET);
            break;
        }

        // Sblocca il mutex del supermercato
        pthread_mutex_unlock(&supermercato->mutex_supermercato);

        sleep(10);  // Attende 10 secondi prima di ricontrollare
    }

    return NULL;
}

// Controlla se possiamo ammettere nuovi clienti
int possiamo_ammettere_clienti(Supermercato *supermercato) {
    printf("il ritorno della funzione \"possiamo ammettere clienti" " è : %d\n", supermercato->clienti_presenti <= supermercato->max_clienti);
    return supermercato->clienti_presenti <= supermercato->max_clienti;
}

// Ammette i clienti dalla lista di attesa
int ammetti_clienti(Supermercato *supermercato) {
    int clienti_da_ammettere = supermercato->max_clienti - supermercato->clienti_presenti;
    int clienti_ammessi = 0;

    // Controlla quanti clienti ci sono effettivamente in lista di attesa e ammette i clienti fino al limite
    for (int i = 0; i < clienti_da_ammettere && i < MAX_CLIENTS && supermercato->lista_attesa[i] != NULL; i++) {
        Cliente *cliente = supermercato->lista_attesa[i];
        
        if (cliente == NULL) {
            continue; // Salta se il cliente è già stato rimosso
        }

        // Aggiungi il cliente al supermercato
        supermercato->clienti_presenti++;
        clienti_ammessi++;

        // Sposta il cliente dalla lista di attesa 
        scegli_cassa_per_cliente(supermercato, cliente);

        // Rimuove il cliente dalla lista di attesa
        supermercato->lista_attesa[i] = NULL;
        printf("Rimuovo il cliente %d dalla lista di attesa.\n", cliente->id);
    }

    // Sposta avanti i clienti rimasti in attesa per mantenere l'ordine FIFO
    printf("Sposto avanti i clienti rimasti in attesa.\n");
    sposta_clienti_avanti(supermercato, clienti_ammessi);
    printf("Il numero di clienti ammessi è : %d\n", clienti_ammessi);
    return clienti_ammessi;
}

// Sposta avanti i clienti rimanenti nella lista di attesa
void sposta_clienti_avanti(Supermercato *supermercato, int clienti_da_ammettere) {
    int j = 0; // nuovo indice per lista ordinata

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (supermercato->lista_attesa[i] != NULL) {
            supermercato->lista_attesa[j++] = supermercato->lista_attesa[i];
        }
    }

    // Imposta i restanti elementi come NULL
    while (j < MAX_CLIENTS) {
        supermercato->lista_attesa[j++] = NULL;
    }
}
//__________________________________________________________________________________________________//
//__________________________________________________________________________________________________//
