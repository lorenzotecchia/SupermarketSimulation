#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../server_include/server_models.h"


int choose_objects(Cliente *cliente) {

    printf("Cliente %d sta scegliendo gli acquisti per %d secondi. [...] \n", cliente->id, cliente->tempo_per_scegliere_oggetti);
    sleep(cliente->tempo_per_scegliere_oggetti);

    // Restituisce il numero di oggetti acquistati
    printf("Cliente %d ha scelto %d oggetti.\n", cliente->id, cliente->numero_di_oggetti);
    return cliente->numero_di_oggetti;
}


void queue_up(Cassa *cassa, Cliente *cliente) {

    pthread_mutex_lock(&cassa->mutex_cassa);

    // Verifica se la coda della cassa ha raggiunto la capacità massima
    if (cassa->num_clienti_in_coda >= MAX_CLIENTS) {
        printf("La coda della cassa %d è piena. Cliente %d non può mettersi in fila.\n", cassa->id, cliente->id);
        pthread_mutex_unlock(&cassa->mutex_cassa); // Sblocca il mutex prima di uscire
        return;
    }

    // Aggiunge il cliente alla coda della cassa
    cassa->coda[cassa->num_clienti_in_coda] = *cliente;
    cassa->num_clienti_in_coda++;

    printf("Cliente %d si è messo in fila alla cassa %d. Clienti in coda: %d\n",
           cliente->id, cassa->id, cassa->num_clienti_in_coda);

    // Segnala al thread della cassa che ci sono clienti in coda
    pthread_cond_signal(&cassa->coda_vuota);

    // Sblocca il mutex della cassa
    pthread_mutex_unlock(&cassa->mutex_cassa);
}
