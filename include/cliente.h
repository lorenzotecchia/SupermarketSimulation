typedef struct {
  int id;
  int tempo_per_scegliere_oggetti;
  int numero_di_oggetti;
} Cliente;

int scegli_oggetti(Cliente *cliente);
void metti_in_fila(Cassa *cassa, Cliente *cliente);
