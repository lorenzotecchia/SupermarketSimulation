#include "cassiere.h"
#include "cliente.h"

typedef struct Cassa{
    Cliente *codaCliente;
    Cassiere *cassiere;
    bool aperta;
}Cassa;