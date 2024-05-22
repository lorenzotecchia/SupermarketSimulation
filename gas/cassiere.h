#ifndef CASSIERE_H
#define CASSIERE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Cassiere {
    int id;
} Cassiere;

Cassiere *newCassiere(int id);
void liberaCassiere(Cassiere *cassiere);

#endif // CASSIERE_H
