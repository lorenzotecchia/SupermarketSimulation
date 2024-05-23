#include "cassa.h"

Cassa *creaCassa(Cassiere *cassiere) {
  Cassa *cassa = (Cassa *)malloc(sizeof(Cassa));
  cassa->codaClienti = createQueue();
  cassa->cassiere = cassiere;
  cassa->tempoAttesa = 0;
  return cassa;
}

void liberaCassa(Cassa *cassa) {
  freeQueue(cassa->codaClienti);
  liberaCassiere(cassa->cassiere);
  free(cassa);
}

void processaClienti(Cassa *cassa) {
  while (!isQueueEmpty(cassa->codaClienti)) {
    Cliente *cliente = dequeue(cassa->codaClienti);
    if (cliente) {
      printf("Cassa %d sta processando un cliente con %d oggetti\n",
             cassa->cassiere->id, cliente->oggetti);
      sleep(cliente->tempo);
      free(cliente);
    }
  }
}
