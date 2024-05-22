#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Cliente {
    int oggetti;
    int tempo; // in secondi
} Cliente;

Cliente *newCliente(int oggetti, int tempo);
void printCliente(Cliente *cliente);

#endif // CLIENTE_H
