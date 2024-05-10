#include "cassiere.h"


Cassiere *newCassiere(){
    Cassiere *c = (Cassiere *)malloc(sizeof(Cassiere));
    c->velocita = rand()%10+1;
    return c;
}