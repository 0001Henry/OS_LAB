// 读者写者问题 读者优先
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 4  // 读者数量
#define NUM_WRITERS 3  // 写者数量

void action(){
    sleep(rand() % 3);
}

sem_t rmutex;  // 用于控制对readercount的互斥访问
sem_t wmutex;  // 用于写操作的互斥访问
int readercount = 0;  // 当前正在读取的读者数量


void *reader(void *param) {
    int id = *((int*)param);
    // int num = 1;
    // while (num--) {
    while(1){
        sem_wait(&rmutex);
        readercount++;
        if (readercount == 1) {
            sem_wait(&wmutex);  // 第一个读者尝试锁定写者锁
        }
        sem_post(&rmutex);

        // 执行读操作
        printf("Reader %d begins to read\n", id);
        action();  // 模拟读取过程
        printf("Reader %d finishes.\n", id);

        sem_wait(&rmutex);
        readercount--;
        if (readercount == 0) {
            sem_post(&wmutex);  // 最后一个读者释放写者锁
        }
        sem_post(&rmutex);
        action();  // 模拟非活动时间
    }
    free(param);
}

void *writer(void *param) {
    int id = *((int*)param);
    while (1) {
        sem_wait(&wmutex);

        printf("Writer %d begins to write.\n", id);
        action();  // 模拟写操作
        printf("Writer %d finishes.\n", id);

        sem_post(&wmutex);
        action();  // 模拟非活动时间
    }
    free(param);
}

int main(void) {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
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

    // 输入q结束
    while (1) {
        char c = getchar();
        if (c == 'q') {
            for (int i = 0; i < NUM_READERS; i++) {
                pthread_cancel(readers[i]);
            }
            for (int j = 0; j < NUM_WRITERS; j++) {
                pthread_cancel(writers[j]);
            }
            break;
        }
    }


    // 销毁信号量
    sem_destroy(&rmutex);
    sem_destroy(&wmutex);

    return 0;
}
