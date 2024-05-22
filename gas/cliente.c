#include "cliente.h"

Cliente *newCliente(int oggetti, int tempo) {
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));
    cliente->oggetti = oggetti;
    cliente->tempo = tempo;
    return cliente;
}

void printCliente(Cliente *cliente) {
    printf("Cliente con %d oggetti, tempo: %d secondi\n", cliente->oggetti, cliente->tempo);
}
