#include "clientGenerator.h"

void main(){
    while(1){
        if(rand()%2==1){
            Cliente *c = newClient();
            printf("Cliente con %d oggetti e %d secondi di tempo\n", c->oggetti, c->tempo);
        }
    }
}