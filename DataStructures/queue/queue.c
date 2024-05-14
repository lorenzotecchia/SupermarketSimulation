#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Funzione per creare una nuova coda
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Funzione per creare un nuovo nodo
Node* createNode(Cliente *cliente) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->cliente = cliente;
    newNode->next = NULL;
    return newNode;
}

// Funzione per aggiungere un cliente alla coda
void enqueue(Queue* queue, Cliente *cliente) {
    Node* newNode = createNode(cliente);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
}

// Funzione per rimuovere un cliente dalla coda
Cliente* dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        fprintf(stderr, "Queue is empty. Cannot dequeue.\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = queue->front;
    Cliente* cliente = temp->cliente;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return cliente;
}

// Funzione per verificare se la coda è vuota
bool isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Funzione per ottenere il cliente al fronte della coda senza rimuoverlo
Cliente* peekQueue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        fprintf(stderr, "Queue is empty. Cannot peek.\n");
        exit(EXIT_FAILURE);
    }
    return queue->front->cliente;
}

// Funzione per deallocare la coda
void freeQueue(Queue* queue) {
    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }
    free(queue);
}
