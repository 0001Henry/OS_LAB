// 读者写者问题 公平型

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5  // 读者数量
#define NUM_WRITERS 3  // 写者数量

void action(){
    sleep(rand() % 3);
}

sem_t S;             // 公平访问的信号量
sem_t rmutex;        // 读者互斥锁
sem_t wmutex;        // 写者互斥锁
int readercount = 0; // 当前读者数量

void *reader(void *param) {
    int id = *((int*)param);
    while (1) {
        sem_wait(&S);  // 试图获取访问权限
        sem_wait(&rmutex);
        if (readercount == 0) {
            sem_wait(&wmutex); // 第一个读者尝试获取写者锁
        }
        readercount++;
        sem_post(&rmutex);
        sem_post(&S);  // 允许其他操作进行

        // 执行读操作
        printf("Reader %d is reading\n", id);
        action();  // 模拟读取过程

        sem_wait(&rmutex);
        readercount--;
        if (readercount == 0) {
            sem_post(&wmutex); // 最后一个读者释放写者锁
        }
        sem_post(&rmutex);

        action();  // 模拟非活动时间
    }
    free(param);
}

void *writer(void *param) {
    int id = *((int*)param);
    while (1) {
        sem_wait(&S);  // 试图获取访问权限
        sem_wait(&wmutex); // 获取写者锁

        // 执行写操作
        printf("Writer %d is writing\n", id);
        action();  // 模拟写操作

        sem_post(&wmutex);
        sem_post(&S);  // 允许其他操作进行

        action();  // 模拟非活动时间
    }
    free(param);
}

int main(void) {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    sem_init(&S, 0, 1);
    sem_init(&rmutex, 0, 1);
    sem_init(&wmutex, 0, 1);

    // 创建读者线程
    for (int i = 0; i < NUM_READERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, reader, id);
    }

    // 创建写者线程
    for (int j = 0; j < NUM_WRITERS; j++) {
        int *id = malloc(sizeof(int));
        *id = j + 1;
        pthread_create(&writers[j], NULL, writer, id);
    }

    // 等待所有线程完成（这在实际情况中通常不会发生，因为线程循环无限）
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int j = 0; j < NUM_WRITERS; j++) {
        pthread_join(writers[j], NULL);
    }

    // 销毁信号量
    sem_destroy(&S);
    sem_destroy(&rmutex);
    sem_destroy(&wmutex);

    return 0;
}
