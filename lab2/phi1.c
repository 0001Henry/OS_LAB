// 哲学家进餐问题 
// 奇数号哲学家先拿左筷后拿右筷；偶数号哲学家则相反

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5 // 哲学家的数目
void action(){
    sleep(rand() % 3);
}

sem_t kuaizi[N];

// 初始化信号量
void initialize_semaphores() {
    for (int i = 0; i < N; i++) {
        sem_init(&kuaizi[i], 0, 1); // 初始化每个筷子信号量为1
    }
}

// 哲学家进餐子程序
void* philosophy(void* num) {
    int i = *(int*)num;
    int left = i; // 左手筷子
    int right = (i + 1) % N; // 右手筷子

    int first, second;
    if(i % 2 == 1){
        first = left;
        second = right;
    }else{
        first = right;
        second = left;
    }

    while (1) {
        printf("Philosopher %d is thinking.\n", i);
        action(); // 模拟思考

        sem_wait(&kuaizi[first]);   // 拿起左手筷子
        sem_wait(&kuaizi[second]);  // 拿起右手筷子

        printf("Philosopher %d is eating.\n", i);
        action(); // 模拟吃饭

        sem_post(&kuaizi[first]);   // 放下左手筷子
        sem_post(&kuaizi[second]);  // 放下右手筷子

    }
}

int main() {
    pthread_t philosophers[N];
    int philosopher_numbers[N];

    initialize_semaphores();

    for (int i = 0; i < N; i++) {
        philosopher_numbers[i] = i;
        pthread_create(&philosophers[i], NULL, philosophy, &philosopher_numbers[i]);
    }

    // 输入q结束
    while (1) {
        char c = getchar();
        if (c == 'q') {
            for (int i = 0; i < N; i++) {
                pthread_cancel(philosophers[i]);
            }
            break;
        }
    }

    return 0;
}
