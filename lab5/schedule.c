//**********************************/
// Name: 衡勇睿
// Data: 2024.12.03
// Description: 磁盘调度算法模拟
//**********************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "schedule.h"

void get_sequence(int seq[], int length) {
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        seq[i] = rand() % DISK_SIZE;
    }
}

int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}


// FCFS 先来先服务调度算法
int fcfs(int seq[], int length, int* pos) {
    int total_seek = 0;
    int cur_pos = *pos;

    for (int i = 0; i < length; i++) {
        printf("%d ", seq[i]);
        total_seek += abs(seq[i] - cur_pos);
        cur_pos = seq[i];
    }
    // 指针指向最后一个请求的位置
    *pos = cur_pos;
    return total_seek;
}


// SSTF 最短寻道时间优先调度算法
// 每次选择距离当前磁头位置最近的请求进行处理
int sstf(int seq[], int length, int* pos) {
    int total_seek = 0;
    int cur_pos = *pos;
    int st[length];
    int cnt = 0;

    for (int i = 0; i < length; i++) {
        st[i] = 0;
    }

    while (cnt < length) {
        int min_dist = DISK_SIZE;
        int next_index = -1;

        for (int i = 0; i < length; i++) {
            if (!st[i] && abs(seq[i] - cur_pos) < min_dist) {
                min_dist = abs(seq[i] - cur_pos);
                next_index = i;
            }
        }

        if (next_index != -1) {
            printf("%d ", seq[next_index]);
            total_seek += min_dist;
            cur_pos = seq[next_index];
            st[next_index] = 1;
            cnt++;
        }
    }

    *pos = cur_pos;
    return total_seek;
}


// SCAN 电梯调度算法
// 在一个方向上处理所有请求，然后反转方向
int scan(int seq[], int length, int* pos) {
    int total_seek = 0;
    int cur_pos = *pos;

    int sorted_seq[length];
    int st[length];
    for (int i = 0; i < length; i++) {
        sorted_seq[i] = seq[i];
        st[i] = 0;
    }
    qsort(sorted_seq, length, sizeof(int), cmp);

    for (int i = 0; i < length; i++) {
        if (sorted_seq[i] >= cur_pos) {
            printf("%d ", sorted_seq[i]);
            total_seek += abs(sorted_seq[i] - cur_pos);
            cur_pos = sorted_seq[i];
            st[i] = 1;
        }
    }
    for (int i = length - 1; i >= 0; i--) {
        if(st[i]) continue;
        if (sorted_seq[i] < cur_pos) {
            printf("%d ", sorted_seq[i]);
            total_seek += abs(sorted_seq[i] - cur_pos);
            cur_pos = sorted_seq[i];
        }
    }
    *pos = cur_pos;
    return total_seek;
}


// CSCAN 循环扫描调度算法
// 在一个方向上处理所有请求，然后返回到最小的请求
int cscan(int seq[], int length, int* pos) {
    int total_seek = 0;
    int init_pos = *pos;
    int cur_pos = init_pos, left = length;

    int sorted_seq[length];
    for (int i = 0; i < length; i++) {
        sorted_seq[i] = seq[i];
    }
    qsort(sorted_seq, length, sizeof(int), cmp);

    for (int i = 0; i < length; i++) {
        if (sorted_seq[i] >= cur_pos) {
            printf("%d ", sorted_seq[i]);
            total_seek += abs(sorted_seq[i] - cur_pos);
            cur_pos = sorted_seq[i];
            left--;
        }
    }

    // 返回到最小的请求
    total_seek += sorted_seq[length - 1] - sorted_seq[0];
    cur_pos = sorted_seq[0];
    printf("%d ", sorted_seq[0]);
    left--;

    for (int i = 1; i < length; i++) {
        if (left) {
            printf("%d ", sorted_seq[i]);
            total_seek += abs(sorted_seq[i] - cur_pos);
            cur_pos = sorted_seq[i];
            left--;
        }
        else break;
    }
    *pos = cur_pos;
    return total_seek;
}


// FSCAN 双队列扫描调度算法
// 将磁盘请求队列分成两个子队列：
// ① 当前所有请求磁盘I/O的进程形成的队列，按SCAN算法处理；
// ② 在扫描期间新出现的所有请求磁盘I/O进程形成的等待队列，本次扫描结束后②添加到①的队尾
// 因为随机产生的序列不包含时序信息，所以无法真正复现FSCAN算法，这里将序列分成两半，分别用于模拟两个子队列，然后分别调用SCAN算法
int fscan(int seq[], int length, int* pos) {
    int len1 = length / 2, len2 = length - len1;
    int total_seek = 0;
    total_seek += scan(seq, len1, pos);
    total_seek += scan(seq + len1, len2, pos);
    return total_seek;
}
