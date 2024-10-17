// 哲学家进餐问题 hyr版
/* 各哲学家拿“满”2支筷子这件事必须互斥的执行。
这就保证了即使一个哲学家在只拿到1支筷子时被阻塞，也不会有别的哲学家会继续尝试拿筷子，此时必有另一个哲学家在吃饭。
这样的话，当前正在吃饭的哲学家放下筷子后，被阻塞的哲学家就可以获得等待的筷子了。
*/

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
sem_t S; // 互斥取筷子

// 初始化信号量
void initialize_semaphores() {
    for (int i = 0; i < N; i++) {
        sem_init(&kuaizi[i], 0, 1); // 初始化每个筷子信号量为1
    }
    sem_init(&S, 0, 1); 
}

// 哲学家进餐子程序
void* philosophy(void* num) {
    int i = *(int*)num;
    int left = i; // 左手筷子
    int right = (i + 1) % N; // 右手筷子

    while (1) {
        printf("Philosopher %d is thinking.\n", i);
        action(); // 模拟思考

        sem_wait(&S); 

        sem_wait(&kuaizi[left]);   // 拿起左手筷子
        sem_wait(&kuaizi[right]);  // 拿起右手筷子

        sem_post(&S); 

        printf("Philosopher %d is eating.\n", i);
        action(); // 模拟吃饭

        sem_post(&kuaizi[left]);   // 放下左手筷子
        sem_post(&kuaizi[right]);  // 放下右手筷子

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

    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
