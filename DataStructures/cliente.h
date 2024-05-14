#include <stdio.h>
#include <stdlib.h>


#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct Cliente{
    int oggetti;
    int tempo; //in secondi
} Cliente;

#endif
Cliente *newCliente();

Cliente *newCliente();

/* Aggiunge aoggetti "cestino del cliente"
 * Non ritorna nulla ma modifica la struct Cliente
 * */
//void aggiungiElementi(Cliente *cliente);

/* Paga gli oggetti mandando ul numero di oggetti da pagare al server
 * Ritorna il flag di corretto invio
 * */
int inviaElementi(int numeroElementi);

void printCliente();

