#ifndef CASSA_H
#define CASSA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"
#include "cassiere.h"

typedef struct Cassa {
    Queue *codaClienti;
    Cassiere *cassiere;
    int tempoAttesa;
} Cassa;

Cassa *creaCassa(Cassiere *cassiere);
void liberaCassa(Cassa *cassa);
void processaClienti(Cassa *cassa);

#endif // CASSA_H
