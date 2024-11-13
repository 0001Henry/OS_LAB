#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100

// 初始化队列
Queue* init_queue() {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = MAX_QUEUE_SIZE;
    queue->front = 0;
    queue->size = 0;
    queue->rear = queue->capacity - 1;  // rear 初始化为容量减1
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

int is_full(Queue* queue) {
    return queue->size == queue->capacity;
}

int is_empty(Queue* queue) {
    return queue->size == 0;
}

void push(Queue* queue, int item) {
    if (is_full(queue)) {
        printf("队列已满，无法入队！\n");
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;  // 环形数组
    queue->array[queue->rear] = item;
    queue->size += 1;
    // printf("进程 %s 入队成功\n", item->process_name);
}

// 出队操作
int pop(Queue* queue) {
    if (is_empty(queue)) {
        printf("队列为空，无法出队！\n");
        // return NULL;
        return -1;
    }
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;  // 环形数组
    queue->size -= 1;
    return item;
}

// 获取队列头部的元素
int get_front(Queue* queue) {
    if (is_empty(queue)) {
        printf("队列为空，没有头部元素！\n");
        // return NULL;
        return -1;
    }
    return queue->array[queue->front];
}

int get_size(Queue* queue) {
    return queue->size;
}

// 删除指定元素的函数
int remove_item(Queue* queue, int item) {
    if (is_empty(queue)) {
        printf("队列为空，无法删除元素！\n");
        return -1;
    }
    int cnt = queue->size;
    while(cnt!=0){
        int front_item = pop(queue);
        if(front_item!=item){
            push(queue,front_item);
        }
        else{
            // printf("元素 %d 已从队列中删除！\n", item);
            return 0;
        }
        cnt--;
    }
    
    // int found = 0;
    // int i = queue->front;
    // int count = 0;
    
    // // 查找指定元素
    // while (count < queue->size) {
    //     if (queue->array[i] == item) {
    //         found = 1;
    //         break;
    //     }
    //     i = (i + 1) % queue->capacity;
    //     count++;
    // }

    // // 如果没有找到元素
    // if (!found) {
    //     printf("元素 %d 不在队列中！\n", item);
    //     return -1;
    // }

    // // 删除元素后将后续元素前移
    // int next_pos = (i + 1) % queue->capacity;
    // while (count < queue->size - 1) {
    //     queue->array[i] = queue->array[next_pos];
    //     i = next_pos;
    //     next_pos = (next_pos + 1) % queue->capacity;
    //     count++;
    // }

    // // 更新 rear 和 size
    // queue->rear = (queue->rear - 1 + queue->capacity) % queue->capacity;
    // queue->size -= 1;

    // // printf("元素 %d 已从队列中删除！\n", item);
    return 0;
}