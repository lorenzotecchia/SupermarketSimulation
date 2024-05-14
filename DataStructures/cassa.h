#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue/queue.h"
#include "cassiere.h"
#include "cliente.h"

typedef struct Cassa{
    Queue *codaClienti;
    Cassiere *cassiere;
    bool aperta;
}Cassa;

Cassa *newCassa();