#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cliente.h"

typedef struct Node {
    Cliente *cliente;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Queue* createQueue();
void enqueue(Queue* queue, Cliente *data);
Cliente *dequeue(Queue* queue);
bool isQueueEmpty(Queue* queue);
Cliente *peekQueue(Queue* queue);
void freeQueue(Queue* queue);
int lunghezzaCoda(Queue *queue);

#endif // QUEUE_H
