//**********************************/
// Name: 衡勇睿
// Data: 2024.12.03
// Description: 磁盘调度算法主程序
//**********************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "schedule.h"

#define SEQUENCE_LENGTH 100

int main() {
    // int seq[SEQUENCE_LENGTH] = {55, 58, 39, 18, 90, 160, 150, 38, 184};
    int seq[SEQUENCE_LENGTH];
    get_sequence(seq, SEQUENCE_LENGTH);

    // 随机数种子设为本人的学号
    srand(22281067);

    printf("Random Disk Access Sequence:\n");
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        printf("%d ", seq[i]);
    }
    printf("\n");

    int total_seek = 0;
    printf("\nFCFS Order:\n");
    // 指定磁头初始位置
    int pos = DISK_SIZE / 2;
    total_seek = fcfs(seq, SEQUENCE_LENGTH, &pos);
    printf("\nTotal Seek Time: %d\n", total_seek);
    printf("Average Seek Time: %.2f\n", (float)total_seek / SEQUENCE_LENGTH);

    printf("\nSSTF Order:\n");
    pos = DISK_SIZE / 2;
    total_seek = sstf(seq, SEQUENCE_LENGTH, &pos);
    printf("\nTotal Seek Time: %d\n", total_seek);
    printf("Average Seek Time: %.2f\n", (float)total_seek / SEQUENCE_LENGTH);

    printf("\nSCAN Order:\n");
    pos = DISK_SIZE / 2;
    total_seek = scan(seq, SEQUENCE_LENGTH, &pos);
    printf("\nTotal Seek Time: %d\n", total_seek);
    printf("Average Seek Time: %.2f\n", (float)total_seek / SEQUENCE_LENGTH);

    printf("\nCSCAN Order:\n");
    pos = DISK_SIZE / 2;
    total_seek = cscan(seq, SEQUENCE_LENGTH, &pos);
    printf("\nTotal Seek Time: %d\n", total_seek);
    printf("Average Seek Time: %.2f\n", (float)total_seek / SEQUENCE_LENGTH);

    printf("\nFSCAN Order:\n");
    pos = DISK_SIZE / 2;
    total_seek = fscan(seq, SEQUENCE_LENGTH, &pos);
    printf("\nTotal Seek Time: %d\n", total_seek);
    printf("Average Seek Time: %.2f\n", (float)total_seek / SEQUENCE_LENGTH);

    return 0;
}