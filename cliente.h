typedef struct {
  int id;
  int tempo_per_scegliere_oggetti;
  int numero_di_oggetti;
} Cliente;

int compra_ogetti(Cliente cliente);

void *metti_in_fila(Cliente cliente);
