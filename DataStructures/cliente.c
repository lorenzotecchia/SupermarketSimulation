#include "cliente.h"

Cliente *newCliente(){
    Cliente *c = (Cliente *)malloc(sizeof(Cliente));
    c->oggetti = rand()%25+1;
    c->tempo = rand()%10+1;
    return c;
}