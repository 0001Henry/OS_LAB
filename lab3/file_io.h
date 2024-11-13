#ifndef FILE_IO_H
#define FILE_IO_H

#include "scheduling.h"
#include "queue.h"

int input_process_data(struct PCB *pcb_list, int process_count, FILE *input_file, int scheduling_type);

void print_schedule(struct PCB *pcb_list, int process_count, FILE *output_file);

void show(int current_time, int current_index, struct PCB pcb_list[], Queue* queue, FILE* output_file);


#endif // FILE_IO_H
