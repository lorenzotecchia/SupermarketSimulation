#include "queue.h"

Queue* createQueue() {
    Queue* queue = (Queue *)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(Queue* queue, Cliente *data) {
    Node* newNode = (Node *)malloc(sizeof(Node));
    newNode->cliente = data;
    newNode->next = NULL;
    if (queue->rear) {
        queue->rear->next = newNode;
    } else {
        queue->front = newNode;
    }
    queue->rear = newNode;
}

Cliente *dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }
    Node* temp = queue->front;
    Cliente *data = temp->cliente;
    queue->front = temp->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    return data;
}

bool isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

Cliente *peekQueue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }
    return queue->front->cliente;
}

void freeQueue(Queue* queue) {
    while (!isQueueEmpty(queue)) {
        free(dequeue(queue));
    }
    free(queue);
}

int lunghezzaCoda(Queue *queue){
    int count = 0;
    Node *temp = queue->front;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}