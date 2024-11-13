#include <stdio.h>
#include "file_io.h"
#include "queue.h"

// 加载数据并初始化PCB
int input_process_data(struct PCB *pcb_list, int process_count, FILE *input_file, int scheduling_type) {
    for (int i = 0; i < process_count; i++) {
        fscanf(input_file, "%s %d %d", pcb_list[i].process_name, &pcb_list[i].arrival_time, &pcb_list[i].burst_time);
        if (scheduling_type == 2) {
            fscanf(input_file, "%d", &pcb_list[i].priority); 
        } else {
            pcb_list[i].priority = 0;  
        }
        pcb_list[i].is_finished = 0;
        pcb_list[i].service_time = 0;
        pcb_list[i].is_in_queue = 0;
        pcb_list[i].start_time = -1;
        pcb_list[i].finish_time = -1;
    }
    return process_count;
}

void print_schedule(struct PCB *pcb_list, int process_count, FILE *output_file) {
    fprintf(output_file, "\n进程名称 | 到达时刻 | 运行时间 | 优先级 | 开始时刻 | 完成时刻 | 周转时间 | 带权周转时间\n");
    
    int total_turnaround_time = 0;
    double total_weighted_turnaround_time = 0.0;

    for (int i = 0; i < process_count; i++) {
        int turnaround_time = pcb_list[i].finish_time - pcb_list[i].arrival_time;
        double weighted_turnaround_time = (double)turnaround_time / pcb_list[i].burst_time;
        
        fprintf(output_file, "%7s | %7d | %8d | %6d | %7d | %8d | %7d | %12.2f\n",
                pcb_list[i].process_name, pcb_list[i].arrival_time, pcb_list[i].burst_time, 
                pcb_list[i].priority, pcb_list[i].start_time, pcb_list[i].finish_time, 
                turnaround_time, weighted_turnaround_time);
        
        total_turnaround_time += turnaround_time;
        total_weighted_turnaround_time += weighted_turnaround_time;
    }
    
    fprintf(output_file, "平均周转时间: %.2f\n", (double)total_turnaround_time / process_count);
    fprintf(output_file, "平均带权周转时间: %.2f\n\n", total_weighted_turnaround_time / process_count);
}


void show(int current_time, int current_index, struct PCB pcb_list[], Queue* queue, FILE* output_file){
    fprintf(output_file, "时间段: %3d 至 %3d \t 正在执行: %s ", current_time, current_time+1, current_index!=-1 ? pcb_list[current_index].process_name:"无");
    fprintf(output_file, "\t就绪队列: ");
    if(is_empty(queue)){
        fprintf(output_file, "空");
    }
    else{
        int i = queue->front;
        int cnt = queue->size;
        while(cnt!=0){
            fprintf(output_file, "%s ", pcb_list[queue->array[i]].process_name);
            i = (i+1)%queue->capacity;
            cnt--;
        }
    }
    fprintf(output_file,"\n");
}
