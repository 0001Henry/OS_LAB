#include <stdio.h>
#include <stdlib.h>
#include "scheduling.h"
#include "file_io.h"
#include "queue.h"

// 按到达时间排序的比较函数（用于FCFS调度）
int compare_by_arrival_time(const void *a, const void *b)
{
    return ((struct PCB *)a)->arrival_time - ((struct PCB *)b)->arrival_time;
}

// 先来先服务调度算法（First-Come, First-Served，FCFS）
void fcfs_scheduling(FILE *input_file, FILE *output_file)
{
    int process_count;       // 进程数量
    int scheduling_type = 0; // 调度类型设为0，表示FCFS调度

    fscanf(input_file, "%d", &process_count); // 从输入文件读取进程数量

    struct PCB pcb_list[process_count];                                       // 创建PCB列表，大小为进程数量
    input_process_data(pcb_list, process_count, input_file, scheduling_type); // 从输入文件读取进程数据

    int current_time = 0;                               // 当前时间
    int finished_process_count = 0, current_index = -1; // 已完成的进程计数，当前进程索引

    Queue *queue = init_queue(); // 初始化进程队列

    while (finished_process_count < process_count){ // 当完成的进程数小于总进程数时，继续调度
        // 检查新到达的进程并入队
        for (int i = 0; i < process_count; i++){
            if (pcb_list[i].is_in_queue == 0 && pcb_list[i].arrival_time == current_time && pcb_list[i].is_finished == 0){
                // printf("process %s 入队\n", pcb_list[i].process_name);
                push(queue, i);              // 将符合条件的进程加入队列
                pcb_list[i].is_in_queue = 1; // 标记进程已在队列中
            }
        }
        // 如果当前有进程在执行，检查是否完成
        if (current_index != -1){
            if (pcb_list[current_index].service_time == pcb_list[current_index].burst_time){
                // 当前进程执行完毕，释放CPU
                pcb_list[current_index].finish_time = current_time; // 设置完成时间
                pcb_list[current_index].is_finished = 1;            // 标记为已完成
                finished_process_count++;                           // 完成的进程计数加1
                current_index = -1; // 清空当前进程
            }
        }
        // 选择下一个执行的进程
        if (current_index == -1 && !is_empty(queue)){
            current_index = pop(queue);                        // 从队首取出下一个进程
            pcb_list[current_index].start_time = current_time; // 设置开始时间
            pcb_list[current_index].is_in_queue = 0;            // 从队列中移除
        }
        // 如果当前有进程在执行，增加其已执行时间
        if (current_index != -1){
            pcb_list[current_index].service_time++;
        }
        show(current_time, current_index, pcb_list, queue, output_file); // 显示当前调度状态
        current_time++;                                                  // 时间推进
    }
    print_schedule(pcb_list, process_count, output_file); // 输出调度结果
}

// SJF（Shortest Job First，最短作业优先）调度算法
void sjf_scheduling(FILE *input_file, FILE *output_file)
{
    int process_count;       // 进程数量
    int scheduling_type = 1; // 调度类型设为1
    fscanf(input_file, "%d", &process_count); // 从输入文件读取进程数量
    struct PCB pcb_list[process_count];                                       // 创建PCB列表，大小为进程数量
    input_process_data(pcb_list, process_count, input_file, scheduling_type); // 从输入文件读取进程数据
    int current_time = 0;                               // 当前时间
    int finished_process_count = 0, current_index = -1; // 已完成的进程计数和当前进程索引
    Queue *queue = init_queue(); // 初始化进程队列
    while (finished_process_count < process_count){ // 当完成的进程数小于总进程数时，继续调度
        // 检查新到达的进程并入队
        for (int i = 0; i < process_count; i++){
            if (pcb_list[i].is_in_queue == 0 && pcb_list[i].arrival_time == current_time && pcb_list[i].is_finished == 0){
                // printf("process %s 入队\n", pcb_list[i].process_name);
                push(queue, i);
                pcb_list[i].is_in_queue = 1; // 标记进程已进入队列中
            }
        }
        // 如果当前有进程在执行，检查是否完成
        if (current_index != -1){
            if (pcb_list[current_index].service_time == pcb_list[current_index].burst_time){
                // 当前进程执行完毕，释放CPU
                pcb_list[current_index].finish_time = current_time; // 设置完成时间
                pcb_list[current_index].is_finished = 1;            // 标记为已完成
                finished_process_count++; // 完成的进程计数加1
                current_index = -1; // 清空当前进程
            }
        }
        // 选择下一个执行的进程
        if (current_index == -1 && !is_empty(queue)){
            int min_time = 0x3f3f3f3f, target_index = -1; // 初始化最短执行时间和目标进程索引
            int i = queue->front;
            int cnt = queue->size;
            // 遍历队列找到执行时间最短的进程
            while (cnt != 0){
                if (pcb_list[queue->array[i]].burst_time < min_time)
                {
                    target_index = queue->array[i];                  // 设置最短执行时间的进程为目标进程
                    min_time = pcb_list[queue->array[i]].burst_time; // 更新最短执行时间
                }
                i = (i + 1) % queue->capacity; // 队列循环遍历
                cnt--;
            }
            // 从队列中移除选定的进程，并将其设为当前执行进程
            remove_item(queue, target_index);
            current_index = target_index;
            pcb_list[current_index].start_time = current_time; // 记录开始时间
            pcb_list[current_index].is_in_queue = 0;  // 标记不在队列中
        }
        // 如果当前有进程在执行，增加其已执行时间
        if (current_index != -1){
            pcb_list[current_index].service_time++;
        }

        show(current_time, current_index, pcb_list, queue, output_file); // 显示当前调度状态
        current_time++;                                                  // 时间推进
    }
    print_schedule(pcb_list, process_count, output_file); // 输出调度结果
}

// 优先级调度算法 (PSA)
void psa_scheduling(FILE *input_file, FILE *output_file){
    int scheduling_type = 2; // 调度类型设为2
    int process_count;       // 进程数量
    fscanf(input_file, "%d", &process_count);

    struct PCB pcb_list[process_count]; // 创建PCB列表，大小为进程数量
    input_process_data(pcb_list, process_count, input_file, scheduling_type);

    int current_time = 0;                               // 当前时间
    int finished_process_count = 0, current_index = -1; // 已完成进程计数，当前进程索引

    Queue *queue = init_queue(); // 初始化队列
    while (finished_process_count < process_count){
        // 新到达的进程入队
        for (int i = 0; i < process_count; i++){
            if (pcb_list[i].is_in_queue == 0 && pcb_list[i].arrival_time == current_time && pcb_list[i].is_finished == 0){
                // printf("process %s 入队\n", pcb_list[i].process_name);
                push(queue, i);              // 将到达的进程加入队列
                pcb_list[i].is_in_queue = 1; // 标记进程已在队列中
            }
        }
        // 如果当前进程已完成
        if (current_index != -1){
            if (pcb_list[current_index].service_time == pcb_list[current_index].burst_time){
                // 进程执行完毕，释放CPU
                pcb_list[current_index].finish_time = current_time; // 设置完成时间
                pcb_list[current_index].is_finished = 1;            // 标记为已完成
                finished_process_count++; // 增加已完成进程计数

                current_index = -1; // 当前进程清空
            }
        }
        // 如果当前没有进程在执行且队列非空
        if (current_index == -1 && !is_empty(queue)){
            int max_priority = -1, target_index = -1; // 初始化最高优先级和目标进程索引
            int i = queue->front;
            int cnt = queue->size;
            // 遍历队列找到优先级最高的进程
            while (cnt != 0){
                if (pcb_list[queue->array[i]].priority > max_priority){
                    target_index = queue->array[i];                    // 设置最高优先级进程
                    max_priority = pcb_list[queue->array[i]].priority; // 更新最大优先级
                }
                i = (i + 1) % queue->capacity; // 队列循环遍历
                cnt--;
            }
            // 从队列中移除优先级最高的进程
            remove_item(queue, target_index);
            current_index = target_index;                      // 设置当前执行进程为最高优先级进程
            pcb_list[current_index].start_time = current_time; // 记录开始时间
            pcb_list[current_index].is_in_queue = 0;  // 从队列中移除
        }

        // 当前进程执行
        if (current_index != -1){
            pcb_list[current_index].service_time++;
        }
        show(current_time, current_index, pcb_list, queue, output_file); // 显示当前调度状态
        current_time++;                                                  // 时间推进
    }
    print_schedule(pcb_list, process_count, output_file); // 输出调度结果
}

// RR（Round Robin）调度算法
void rr_scheduling(FILE *input_file, FILE *output_file){
    int scheduling_type = 3;                                    // 调度类型为3
    int process_count;                                          // 进程数量
    int time_quantum;                                           // 时间片大小
    fscanf(input_file, "%d %d", &process_count, &time_quantum); // 从输入文件读取进程数量和时间片大小
    struct PCB pcb_list[process_count];                                       // 创建PCB列表，大小为进程数量
    input_process_data(pcb_list, process_count, input_file, scheduling_type); // 从输入文件读取进程数据
    int current_time = 0, finished_process_count = 0; // 当前时间和已完成的进程计数
    int current_index = -1;                           // 当前正在CPU中的进程索引
    int tiny_time = 0;                                // 当前进程在CPU中已经执行的时间
    Queue *queue = init_queue(); // 初始化进程队列
    while (finished_process_count < process_count){ // 当完成的进程数小于总进程数时，继续调度
        // 检查新到达的进程，将其入队
        for (int i = 0; i < process_count; i++){
            if (pcb_list[i].is_in_queue == 0 && pcb_list[i].arrival_time == current_time && pcb_list[i].is_finished == 0){
                // printf("process %d 入队\n", i);
                push(queue, i);              // 将到达的进程加入队列
                pcb_list[i].is_in_queue = 1; // 标记该进程已在队列中
            }
        }
        if (current_index != -1){ // 如果当前有正在执行的进程
            if (pcb_list[current_index].service_time == pcb_list[current_index].burst_time){  // 如果当前进程执行完成
                pcb_list[current_index].finish_time = current_time; // 设置完成时间
                pcb_list[current_index].is_finished = 1;            // 标记进程完成
                pcb_list[current_index].is_in_queue = 0;            // 标记进程不在队列中
                finished_process_count++;                           // 完成的进程计数加1
                current_index = -1; // 清空当前进程索引
                if (!is_empty(queue)){
                    current_index = pop(queue); // 队首进程进入CPU
                    if (pcb_list[current_index].start_time == -1){
                        pcb_list[current_index].start_time = current_time; // 设置开始时间
                    }
                    tiny_time = 0; // 重置时间片计数
                }
            }
            else{ // 如果当前进程未执行完
                if (tiny_time == time_quantum){ // 时间片用完，切换进程
                    push(queue, current_index); // 将当前进程放回队列尾部
                    current_index = pop(queue); // 队首进程进入CPU
                    if (pcb_list[current_index].start_time == -1){
                        pcb_list[current_index].start_time = current_time; // 设置开始时间
                    }
                    tiny_time = 0; // 重置时间片计数
                }
            }
        }
        else{ // 当前无进程在CPU中
            if (!is_empty(queue)){
                current_index = pop(queue); // 队首进程进入CPU
                if (pcb_list[current_index].start_time == -1){
                    pcb_list[current_index].start_time = current_time; // 设置开始时间
                }
                // tiny_time = 0; // 重置时间片计数
            }
        }
        if (current_index != -1){                                        // 如果有进程在CPU中执行
            pcb_list[current_index].service_time++;                      // 增加当前进程的已执行时间
        }
        show(current_time, current_index, pcb_list, queue, output_file); // 显示当前调度队列状态
        current_time++;                                                  // 时间推进
        tiny_time++;                                                     // 增加时间片计数
    }
    print_schedule(pcb_list, process_count, output_file); // 输出调度结果
}

// Shortest Job First (SJF) scheduling algorithm
void sjf_scheduling_old(FILE *input_file, FILE *output_file)
{
    int process_count;
    int scheduling_type = 1;
    fscanf(input_file, "%d", &process_count);

    struct PCB pcb_list[process_count];
    input_process_data(pcb_list, process_count, input_file, scheduling_type);

    qsort(pcb_list, process_count, sizeof(struct PCB), compare_by_arrival_time);

    int current_time = 0;
    for (int i = 0; i < process_count; i++)
    {
        int selected_index = -1, min_burst_time = 0x3f3f3f3f;
        for (int j = 0; j < process_count; j++)
        {
            if (pcb_list[j].arrival_time <= current_time && !pcb_list[j].is_finished && pcb_list[j].burst_time < min_burst_time)
            {
                selected_index = j;
                min_burst_time = pcb_list[j].burst_time;
            }
        }
        if (selected_index != -1)
        {
            pcb_list[selected_index].is_finished = 1;
            pcb_list[selected_index].start_time = (current_time > pcb_list[selected_index].arrival_time) ? current_time : pcb_list[selected_index].arrival_time;
            current_time += pcb_list[selected_index].burst_time;
            pcb_list[selected_index].finish_time = current_time;
        }
    }

    print_schedule(pcb_list, process_count, output_file);
}

// Round Robin (RR) scheduling algorithm
void rr_scheduling_old(FILE *input_file, FILE *output_file)
{
    int process_count;
    int scheduling_type = 3;
    fscanf(input_file, "%d", &process_count);

    int time_quantum;
    fscanf(input_file, "%d", &time_quantum);

    struct PCB pcb_list[process_count];
    input_process_data(pcb_list, process_count, input_file, scheduling_type);

    qsort(pcb_list, process_count, sizeof(struct PCB), compare_by_arrival_time);

    int current_time = 0, finished_process_count = 0, index = 0;

    while (finished_process_count < process_count)
    {
        if (!pcb_list[index].is_finished && pcb_list[index].arrival_time <= current_time)
        {
            if (pcb_list[index].service_time == 0)
            {
                pcb_list[index].start_time = current_time;
            }
            if (pcb_list[index].burst_time - pcb_list[index].service_time <= time_quantum)
            {
                current_time += pcb_list[index].burst_time - pcb_list[index].service_time;
                pcb_list[index].finish_time = current_time;
                pcb_list[index].is_finished = 1;
                finished_process_count++;
            }
            else
            {
                current_time += time_quantum;
                pcb_list[index].service_time += time_quantum;
            }
        }
        index = (index + 1) % process_count;
    }

    print_schedule(pcb_list, process_count, output_file);
}

// Priority Scheduling Algorithm (PSA)
void psa_scheduling_old(FILE *input_file, FILE *output_file)
{
    int process_count;
    int scheduling_type = 2;
    fscanf(input_file, "%d", &process_count);

    struct PCB pcb_list[process_count];
    input_process_data(pcb_list, process_count, input_file, scheduling_type);

    qsort(pcb_list, process_count, sizeof(struct PCB), compare_by_arrival_time);

    int current_time = 0;
    for (int i = 0; i < process_count; i++)
    {
        int selected_index = -1, max_priority = -1;
        for (int j = 0; j < process_count; j++)
        {
            if (pcb_list[j].arrival_time <= current_time && !pcb_list[j].is_finished && pcb_list[j].priority > max_priority)
            {
                selected_index = j;
                max_priority = pcb_list[j].priority;
            }
        }
        if (selected_index != -1)
        {
            pcb_list[selected_index].is_finished = 1;
            pcb_list[selected_index].start_time = (current_time > pcb_list[selected_index].arrival_time) ? current_time : pcb_list[selected_index].arrival_time;
            current_time += pcb_list[selected_index].burst_time;
            pcb_list[selected_index].finish_time = current_time;
        }
    }

    print_schedule(pcb_list, process_count, output_file);
}
