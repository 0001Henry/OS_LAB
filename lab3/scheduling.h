#ifndef SCHEDULING_H 
#define SCHEDULING_H

#include <stdio.h>

// 定义进程控制块 (PCB) 结构体
struct PCB {
    char process_name[5];  // 进程名称
    int arrival_time;      // 到达时间，表示进程何时到达系统
    int priority;          // 优先级，用于优先级调度算法
    int burst_time;        // CPU运行时间，这里表示进程的总需求执行时间
    int start_time;        // 开始时间，记录进程首次进入CPU的时间，初始化为-1
    int finish_time;       // 完成时间，记录进程完成执行的时间，初始化为-1
    int is_finished;       // 完成标志，1表示完成，0表示未完成
    int service_time;      // 服务时间，用于记录进程已在CPU上运行的时间（RR算法用）
    int is_in_queue;       // 队列标志，1表示当前在队列中，0表示不在队列中
};

// 调度算法的声明
void fcfs_scheduling(FILE *input_file, FILE *output_file);  // 先来先服务调度算法
void sjf_scheduling(FILE *input_file, FILE *output_file);   // 最短作业优先调度算法
void psa_scheduling(FILE *input_file, FILE *output_file);   // 优先级调度算法
void rr_scheduling(FILE *input_file, FILE *output_file);    // 时间片轮转调度算法

#endif 
