#include <pthread.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdatomic.h>

// Definizione colori per il terminale
#define RESET_COLOR    "\x1b[0m"
#define GREEN_COLOR    "\x1b[32m"
#define BLUE_COLOR     "\x1b[34m"
#define CYAN_COLOR     "\x1b[36m"

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100
#define MAX_CASHIERS 20

typedef struct {
  int id;
  int tempo_per_scegliere_oggetti;
  int numero_di_oggetti;
} Cliente;

typedef struct {
    void *cassa;
    void *supermercato;
} ParametriCassa;

typedef struct {
  int id;
  int tempo_fisso;
  int tempo_totale;
  pthread_mutex_t mutex_cassa;
  pthread_cond_t coda_vuota;
  Cliente coda[MAX_CLIENTS];
  atomic_int num_clienti_in_coda;
} Cassa;

typedef struct {
  int num_casse;
  int max_clienti;
  int clienti_presenti;
  int clienti_fuori;
  Cliente *lista_attesa[MAX_CLIENTS]; // FIFO dei clienti fuori dal supermercato
  Cassa *casse[MAX_CASHIERS];
  pthread_mutex_t mutex_supermercato;
  pthread_cond_t spazio_disponibile;
} Supermercato;

typedef struct pthread_arg_t {
    Supermercato *supermercato;
    int new_socket_fd;
    struct sockaddr_in client_address;
} pthread_arg_t;

// Funzioni per il cliente
int scegli_oggetti(Cliente *cliente);
void metti_in_fila(Cassa * cassa, Cliente *cliente);

// Funzioni per la cassa
void *servi_cliente(void *arg);

// Funzioni per il supermercato
void inizializza_supermercato(Supermercato *supermercato, int num_casse, int max_clienti);
void scegli_cassa_per_cliente(Supermercato *supermercato, Cliente *cliente);
int trova_cassa_minima(Supermercato *supermercato);
void* supervisiona_supermercato(void *arg);
int possiamo_ammettere_clienti(Supermercato *supermercato);
int ammetti_clienti(Supermercato *supermercato);
void sposta_clienti_avanti(Supermercato *supermercato, int clienti_da_ammettere);
void print_welcome_message(int num_casse, int server_port);

// Funzioni di connessione al client
void setup_server_socket(int server_port, int *socket_fd);
void accept_connections(int socket_fd, Supermercato *supermercato);
void signal_handler(int signal_number);
void *client_handler(void *arg);
