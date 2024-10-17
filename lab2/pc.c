// 生产者-消费者问题
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PRODUSERS 4
#define NUM_CONSUMERS 3

int n = 6; // 缓冲区大小
int buffer[6]; // 缓冲区

int in = 0; // 输入指针
int out = 0; // 输出指针

// 互斥信号量
sem_t mutex; 

// 资源信号量
sem_t empty; // 空缓存区数量，初始化为n
sem_t full; // 满缓存区数量, 初始化为0


void* producer(void* num) {
    int i = *(int*)num;
    int nextp;

    while(1){
        nextp = i; 
        sleep(rand()%3); // 模拟生产过程

        sem_wait(&empty); // 资源信号量在前
        sem_wait(&mutex); // 互斥信号量在后 

        buffer[in] = nextp;
        printf("生产者 %d 在缓冲区 %d 放入了资料 %d。\n", i, in, nextp);
        in = (in + 1)%n;

        sem_post(&mutex);
        sem_post(&full);

    }
}

void* consumer(void* num) {
    int i = *(int*)num;
    int nextp;

    while(1){

        sem_wait(&full);  // 资源信号量在前      
        sem_wait(&mutex); // 互斥信号量在后 

        nextp = buffer[out];
        printf("消费者 %d 从缓冲区 %d 取出了资料 %d。\n", i, out, nextp);
        out = (out + 1)%n;

        sem_post(&mutex);
        sem_post(&empty); 

        sleep(rand() % 3); // 模拟消费过程
    }
}

int main(void) {
    pthread_t producers[NUM_PRODUSERS], consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUSERS], consumer_ids[NUM_CONSUMERS];

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, n);
    sem_init(&full, 0, 0);

    // 创建生产者线程
    for (int i = 0; i < NUM_PRODUSERS; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // 创建消费者线程
    for (int j = 0; j < NUM_CONSUMERS; j++) {
        consumer_ids[j] = j + 1;
        pthread_create(&consumers[j], NULL, consumer, &consumer_ids[j]);
    }

    // 输入q结束
    while (1) {
        char c = getchar();
        if (c == 'q') {
            for (int i = 0; i < NUM_PRODUSERS; i++) {
                pthread_cancel(producers[i]);
            }
            for (int j = 0; j < NUM_CONSUMERS; j++) {
                pthread_cancel(consumers[j]);
            }
            break;
        }
    }

    // 销毁信号量
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
