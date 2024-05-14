#include "cliente.h"

void aggiungiElementi(Cliente *cliente) { cliente->oggetti = rand() % 25 + 1; }

Cliente *newCliente() {
  Cliente *c = (Cliente *)malloc(sizeof(Cliente));
  aggiungiElementi(c);
  c->tempo = rand() % 10 + 1;
  return c;
}

void printCliente(Cliente *c) {
  printf("Cliente con %d oggetti e %d secondi di tempo\n", c->oggetti, c->tempo);
}
