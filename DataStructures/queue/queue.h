#include <stdio.h>
#include <stdlib.h>
#include "../cliente.h"

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

// Definizione del nodo
typedef struct Node {
    Cliente *cliente;
    struct Node* next;
} Node;

// Definizione della coda
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// Funzione per creare una nuova coda
Queue* createQueue();

// Funzione per aggiungere un elemento alla coda
void enqueue(Queue* queue, Cliente *data);

// Funzione per rimuovere un elemento dalla coda
Cliente *dequeue(Queue* queue);

// Funzione per verificare se la coda è vuota
bool isQueueEmpty(Queue* queue);

// Funzione per ottenere l'elemento al fronte della coda senza rimuoverlo
Cliente *peekQueue(Queue* queue);

// Funzione per deallocare la coda
void freeQueue(Queue* queue);

#endif // QUEUE_H
