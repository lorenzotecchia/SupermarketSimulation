#include "minHeap.h"

// La funzione heapify viene utilizzata per costruire un heap a partire
// da un singolo nodo radice.
void heapify(int arr[], int n, int i) {
  int smallest = i;
  int l = 2 * i + 1; // il figlio sinistro di i si trova in posizione = 2*i + 1
  int r = 2 * i + 2; // il figlio destro di i si trova in posizione = 2*i + 2

  // Verifica se il figlio sinistro è più piccolo del padre
  if (l < n && arr[i] > arr[l])
    smallest = l;

  // Verifica se il figlio destro è più piccolo del padre o del
  // figlio sinistro
  if (r < n && arr[smallest] > arr[r])
    smallest = r;

  // Se il padre non è il più piccolo, scambia il padre con il
  // figlio più piccolo e richiama heapify per il figlio appena scambiato
  if (smallest != i) {
    int temp = arr[i];
    arr[i] = arr[smallest];
    arr[smallest] = temp;
    heapify(arr, n, smallest);
  }
}

// La funzione heapSort utilizza heapify per costruire l'heap iniziale
// e ordinare l'array.
void heapSort(int arr[], int n) {
  // Costruisci un heap a partire da un nodo radice e dai suoi figli.
  // L'indice di partenza è n / 2 - 1 in modo da includere solo nodi
  // con figli.
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);

  // Estrai il nodo radice (l'elemento più piccolo) e ricostruisci
  // l'heap con gli elementi rimanenti.
  // In questo modo si ordinano gli elementi in ordine crescente.
  for (int i = n - 1; i > 0; i--) {
    int temp = arr[0];
    arr[0] = arr[i];
    arr[i] = temp;
    heapify(arr, i, 0);
  }
  printf("--MIN HEAP SORT--\n");
}

void printArray(int arr[], int n) {
  for (int i = 0; i < n; i++) {
    printf("%d\t", arr[i]);
  }
  printf("\n");
}

void buildHeap(int arr[], int n) {
  printf("------ BUILD HEAP ------\n");
  for (int i = n / 2 - 1; i >= 0; i--) {
    heapify(arr, n, i);
  }
}
