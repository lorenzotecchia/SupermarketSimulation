#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define WIDTH 60
#define HEIGHT 30

// Simboli per l'omino e la struttura del supermercato
#define OMINO "O"
#define WALL "#"
#define DOOR " "

// Colore blu (sequenza ANSI)
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

void print_welcome_message();
void print_shopping_message(int client_id);
void print_waiting_in_queue_message(int client_id);
void print_closing_message();
