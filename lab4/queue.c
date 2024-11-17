#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// 初始化队列
void initQueue(Queue* q, int size) {
    q->data = (Item*)malloc(sizeof(Item) * size);
    q->front = q->rear = 0;
    q->count = 0;
}

// 释放队列
void freeQueue(Queue* q) {
    free(q->data);
}

// 判断队列是否为空
int isEmpty(Queue* q) {
    return q->count == 0;
}
 
// 判断队列是否已满
int isFull(Queue* q) {
    return q->count == q->size;
}
 
// 入队操作
void enqueue(Queue* q, Item item) {
    if (isFull(q)) {
        printf("队列已满\n");
        return;
    }
    q->data[q->rear] = item;
    q->rear = (q->rear + 1) % q->size;
    q->count++;
}
 
// 出队操作
Item dequeue(Queue* q) {
    if (isEmpty(q)) {
        printf("队列为空\n");
        exit(1);
    }
    Item item = q->data[q->front];
    q->front = (q->front + 1) % q->size;
    q->count--;
    return item;
}
 

// int main() {
//     Queue q;
//     initQueue(&q);
 
//     enqueue(&q, 1);
//     enqueue(&q, 2);
//     printf("出队: %d\n", dequeue(&q));
//     enqueue(&q, 3);
//     printf("出队: %d\n", dequeue(&q));
//     printf("出队: %d\n", dequeue(&q));
 
//     return 0;
// }