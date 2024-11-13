#ifndef QUEUE_H
#define QUEUE_H

#include<stdio.h>

typedef struct {
    int *array;    // 指向 int 指针的数组
    int front;             // 队列头
    int rear;              // 队列尾
    int size;              // 当前队列大小
    int capacity;          // 队列容量
} Queue;

Queue* init_queue();
int is_full(Queue* queue);
int is_empty(Queue* queue);
void push(Queue* queue, int item);
int pop(Queue* queue);
int get_front(Queue* queue);
int get_size(Queue* queue);
// 删除指定元素的函数
int remove_item(Queue* queue, int item);

#endif

