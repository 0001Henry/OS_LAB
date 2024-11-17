# pragma once

#include <stdio.h>
#include <stdlib.h>
 
#define MAX_QUEUE_SIZE 100
 
typedef int Item;
 
typedef struct Queue {
    Item* data;
    int front;
    int rear;
    int count;
    int size;
} Queue;
 
// 初始化队列
void initQueue(Queue* q, int size);
 
// 释放队列
void freeQueue(Queue* q);

// 判断队列是否为空
int isEmpty(Queue* q);
 
// 判断队列是否已满
int isFull(Queue* q);

// 入队操作
void enqueue(Queue* q, Item item);
 
// 出队操作
Item dequeue(Queue* q);