#ifndef CASSA_H
#define CASSA_H

#include "cassiere.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _cassa {
  Queue *codaClienti;
  Cassiere *cassiere;
  int tempoAttesa;
} Cassa;

Cassa *creaCassa(Cassiere *cassiere);
void liberaCassa(Cassa *cassa);
void processaClienti(Cassa *cassa);

#endif // CASSA_H
