#include "cassiere.h"

Cassiere *newCassiere(int id) {
    Cassiere *cassiere = (Cassiere *)malloc(sizeof(Cassiere));
    cassiere->id = id;
    return cassiere;
}

void liberaCassiere(Cassiere *cassiere) {
    free(cassiere);
}
