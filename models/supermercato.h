#include "cassa.h"
#include "cliente.h"
#include <pthread.h>

#define MAX_CASHIERS 20

typedef struct {
  int num_casse;
  int max_clienti;
  int clienti_presenti;
  int clienti_fuori;
  int E;
  Cliente *lista_attesa[MAX_CLIENTS]; // FIFO dei clienti fuori dal supermercato
  Cassa casse[MAX_CASHIERS];
  pthread_mutex_t mutex_supermercato;
  pthread_cond_t spazio_disponibile;
} Supermercato;
