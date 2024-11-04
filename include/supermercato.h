#include "cassa.h"
#include "cliente.h"
#include <pthread.h>
#define BUFFER_SIZE 1024
#define MAX_CASHIERS 20

typedef struct {
  int num_casse;
  int max_clienti;
  int clienti_presenti;
  int clienti_fuori;
  int E;
  Cliente *lista_attesa[MAX_CLIENTS]; // FIFO dei clienti fuori dal supermercato
  Cassa *casse[MAX_CASHIERS];
  pthread_mutex_t mutex_supermercato;
  pthread_cond_t spazio_disponibile;
} Supermercato;

void inizializza_supermercato(Supermercato *supermercato, int num_casse, int max_clienti, int E);
void scegli_cassa_per_cliente(Supermercato *supermercato, Cliente *cliente);
int trova_cassa_minima(Supermercato *supermercato);
void* supervisiona_supermercato(void *arg);
int possiamo_ammettere_clienti(Supermercato *supermercato);
int ammetti_clienti(Supermercato *supermercato);
void sposta_clienti_avanti(Supermercato *supermercato, int clienti_da_ammettere);