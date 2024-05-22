#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void main() {
  srand(time(NULL));
  while (1) {
    if (rand() % 2 == 1) {
      Cliente *c = newCliente();
      printf("spm");
    }
    usleep(100);
  }
}
