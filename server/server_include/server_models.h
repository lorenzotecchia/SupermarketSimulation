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
int choose_objects(Cliente *cliente);
void queue_up(Cassa * cassa, Cliente *cliente);

// Funzioni per la cassa
void *serve_clients(void *arg);

// Funzioni per il supermercato
void initialize_supermarket(Supermercato *supermercato, int num_casse, int max_clienti);
void choose_checkout_for_client(Supermercato *supermercato, Cliente *cliente);
int find_minimum_checkout(Supermercato *supermercato);
void* manage_supermarket(void *arg);
int clients_to_be_admitted(Supermercato *supermercato);
int admit_clients(Supermercato *supermercato);
void move_clients_forward(Supermercato *supermercato, int clienti_da_ammettere);
void print_welcome_message(int num_casse, int server_port);

// Funzioni di connessione al client
void setup_server_socket(int server_port, int *socket_fd);
void accept_connections(int socket_fd, Supermercato *supermercato);
void signal_handler(int signal_number);
void *client_handler(void *arg);
