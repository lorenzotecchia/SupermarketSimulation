#include "cliente.h"
#include <pthread.h>
#define MAX_CLIENTS 100

typedef struct {
  int id;
  int tempo_fisso; // tempo del cassiere
  int tempo_totale; // tempo attuale di servizio totale
  pthread_mutex_t mutex_cassa;
  pthread_cond_t coda_vuota;
  Cliente *coda[MAX_CLIENTS]; // FIFO dei clienti in attesa
  int num_clienti_in_coda;
} Cassa;

void* servi_cliente(void *arg);