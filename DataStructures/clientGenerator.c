#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "cliente.h"


void main(){
    srand(time(NULL));
    while(1){
        if(rand()%2==1){
            Cliente *c = newCliente();
        }
        sleep(1);
    }
}