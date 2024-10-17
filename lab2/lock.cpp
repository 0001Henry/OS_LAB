#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

# define NUM_PEOPLE 5

int W=0, wcount = 0;
sem_t ws;

// 上锁
void Lock(int &W)
{
    if (W==1)
    {
        wcount++;
        sem_wait(&ws);
    }
    W = 1;
}

// 解锁
void Unlock(int &W)
{
    if (wcount > 0)
    {
        wcount--;
        sem_post(&ws);
    }
    W = 0;
}


void* person(void* num) {
    int i = *(int*)num;

    while(1){
        Lock(W);
        printf("person %d is entering the room\n", i);
        sleep(rand()%3);
        Unlock(W);
        printf("person %d is leaving the room\n", i);
        sleep(rand()%3);
    }   
}


int main(){
    pthread_t people[NUM_PEOPLE];

    // 创建线程
    for (int i = 0; i < NUM_PEOPLE; i++) {
        int *id = (int*)malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&people[i], NULL, person, id);
    }


    // 输入q结束进程
    while (1) {
        char c = getchar();
        if (c == 'q') {
            for (int i = 0; i < NUM_PEOPLE; i++) {
                pthread_cancel(people[i]);
            }
            break;
        }
    }
    // 销毁信号量
    sem_destroy(&ws);

    return 0;
}