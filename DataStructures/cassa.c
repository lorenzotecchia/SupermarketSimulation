#include "cassa.h"

#define MAX_QUEUE 10

Cassa *newCassa() {
  Cassa *c = (Cassa *)malloc(sizeof(Cassa));
  c->codaClienti = createQueue();
  c->cassiere = newCassiere();
  c->aperta = true;
  return c;
}

int main() {
  Cassa *cassa1 = newCassa();
  printf("Cassa aperta: %d\n", cassa1->aperta);
  printf("Velocita' cassiere: %d\n", cassa1->cassiere->velocita);
  for (int i = 0; i < MAX_QUEUE; i++) {
    enqueue(cassa1->codaClienti, newCliente());
  }
  for (int i = 0; i < MAX_QUEUE; i++) {
    Cliente *c = dequeue(cassa1->codaClienti);
    printCliente(c);
  }
  return 0;
}
