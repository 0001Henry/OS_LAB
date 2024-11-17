#include "func.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"

// 初始化工作集
void initialize_working_set(WorkingSet *working_set, int working_set_size, int start_page_id, int total_page_num, int speed, int access_length){
    working_set->working_set_size = working_set_size;
    working_set->loaded_pages = (int *)malloc(working_set_size * sizeof(int));
    memset(working_set->loaded_pages, -1, working_set_size * sizeof(int));  // 初始化为-1

    working_set->start_page_id = start_page_id;
    working_set->speed = speed;

    working_set->access_length = access_length;

    working_set->access_page_ids = (int *)malloc(access_length * sizeof(int));
    working_set->access_modes = (int *)malloc(access_length * sizeof(int));

    working_set->total_page_num = total_page_num;
    working_set->pages = (Page *)malloc(total_page_num * sizeof(Page));
    working_set->page_fault_num = 0;
    working_set->total_access_num = 0;
    
    // 初始化页面信息
    for (int i = 0; i < total_page_num; i++){
        // 70%页面支持读写，30%页面只读
        double tmp = (double)rand() / RAND_MAX;
        if (tmp > 0.7){
            working_set->pages[i].access_mode = PAGE_READ_ONLY;
        }
        else{
            working_set->pages[i].access_mode = PAGE_READ_WRITE;
        }

        // 模拟随机分配到的物理块号
        working_set->pages[i].block_id = 12 + total_page_num - i;

        working_set->pages[i].is_accessed = 0;
        working_set->pages[i].is_modified = 0;
    }
}

// 释放工作集
void free_working_set(WorkingSet *working_set){
    free(working_set->loaded_pages);
    free(working_set->access_page_ids);
    free(working_set->access_modes);
    free(working_set->pages);
}

// 生成访问序列
void generate_access_sequence(WorkingSet *working_set){
    for(int i = 0; i < working_set->access_length;){
        // 生成访问序列
        for (int j = 0; j < working_set->speed && i < working_set->access_length; j++, i++)
        {
            int random_min = working_set->start_page_id;
            int random_max = working_set->start_page_id + working_set->total_page_num;
            if (random_max > working_set->total_page_num - 1)
                random_max = working_set->total_page_num - 1;

            // 随机选择页面ID
            int page_id = rand() % (random_max - random_min + 1) + random_min;
            working_set->access_page_ids[i] = page_id;

            // 随机选择访问模式
            double r = (double)rand() / RAND_MAX;
            if (r > 0.7 && working_set->pages[page_id].access_mode == PAGE_READ_WRITE)
                working_set->access_modes[i] = ACCESS_WRITE;
            else
                working_set->access_modes[i] = ACCESS_READ;
        }

        double t = 0.3; // 30%的概率
        // 工作集移动
        double r = (double)rand() / RAND_MAX;
        if (r < t)
            working_set->start_page_id = rand() % working_set->total_page_num;
        else
            working_set->start_page_id = (working_set->start_page_id + 1) % working_set->total_page_num;
    }
}

// 打印已加载的页面
void print_loaded_pages(WorkingSet *working_set){
    fprintf(stdout, "内存页面分布：[");
    for (int i = 0; i < working_set->working_set_size; i++)
    {
        if (working_set->loaded_pages[i] == -1)
            fprintf(stdout, " N");
        else
            fprintf(stdout, " %d", working_set->loaded_pages[i]);
    }
    fprintf(stdout, " ]");
}

// 检查指定的页面是否已装入
int find_page(WorkingSet *working_set, int page_id){
    for (int i = 0; i < working_set->working_set_size; i++){
        if (working_set->loaded_pages[i] == page_id)
            return i;
    }
    return -1; // 页面不在内存中
}


// 最优页面置换算法（OPT）
void optimal_page_replacement(WorkingSet *working_set){
    int loaded_cnt = 0;
    double total_time = 0;

    clock_t start_time, end_time;

    // 预装入阶段
    for (int i = 0;i < PRELOADED_NUM && i < working_set->working_set_size; i++){
        int flag = 0;
        for (int j = 0; j < loaded_cnt; j++){
            if (working_set->loaded_pages[j] == working_set->access_page_ids[i]){
                flag = 1;
                break;
            }
        }

        if (!flag){
            working_set->loaded_pages[loaded_cnt++] = working_set->access_page_ids[i];
        }
    }

    for (int i = 0; i < working_set->access_length; i++){
        if (working_set->access_modes[i] == ACCESS_READ)
            fprintf(stdout, "访问方式：读\t");
        else if (working_set->access_modes[i] == ACCESS_WRITE)
            fprintf(stdout, "访问方式：写\t");
        else
            fprintf(stderr, "错误：未知的访问方式!\n");

        fprintf(stdout, "页号：%4d\t物理块号：%4d\t", working_set->access_page_ids[i], working_set->pages[working_set->access_page_ids[i]].block_id);

        // 预装入阶段
        if (i < PRELOADED_NUM && i < working_set->working_set_size){
            // int flag = 0;
            // for (int j = 0; j < loaded_cnt; j++){
            //     if (working_set->loaded_pages[j] == working_set->access_page_ids[i]){
            //         flag = 1;
            //         break;
            //     }
            // }

            // if (!flag){
            //     working_set->loaded_pages[loaded_cnt++] = working_set->access_page_ids[i];
            // }

            working_set->total_access_num++;
            print_loaded_pages(working_set);
            fprintf(stdout, "\t预装入内存\n");
            continue;
        }

        // 页面中断
        int page_index = find_page(working_set, working_set->access_page_ids[i]);
        if (page_index != -1){
            // 页面已装入
            print_loaded_pages(working_set);
            working_set->total_access_num++;
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t页面已在内存\n");
        }
        else{
            // 页面未装入
            start_time = clock();

            int replace_id = 0;
            // 如果工作集未装满
            if(loaded_cnt < working_set->working_set_size){
                replace_id = loaded_cnt;
                loaded_cnt++;
            }
            else{
                
                int res = -1;
                int tmp;
                // 记录每个页面的在i时刻后的最早访问时刻
                for (int j = 0; j < working_set->working_set_size; j++){
                    for (int k = i+1; k < working_set->access_length; k++){
                        tmp = k;
                        if (working_set->access_page_ids[k] == working_set->loaded_pages[j]){    
                            break;
                        }
                    }
                    if (tmp > res){
                        res = tmp;
                        replace_id = j;
                    }
                }
                
            }

            working_set->loaded_pages[replace_id] = working_set->access_page_ids[i];
            working_set->page_fault_num++;
            working_set->total_access_num++;

            end_time = clock();
            total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

            print_loaded_pages(working_set);
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t触发缺页中断\n");
        }
    }

    fprintf(stdout, "缺页中断次数：%d\n", working_set->page_fault_num);
    fprintf(stdout, "缺页率：%.2lf%%\n", (double)(working_set->page_fault_num) / working_set->total_access_num * 100);
    fprintf(stdout, "淘汰页查找时间开销：%lfs\n", total_time);
}

// 先进先出页面置换算法（FIFO）
void fifo_page_replacement(WorkingSet *working_set){
    int loaded_cnt = 0;
    double total_time = 0;

    clock_t start_time, end_time;

    Queue queue;
    initQueue(&queue, working_set->working_set_size);

    // 预装入阶段
    for (int i = 0;i < PRELOADED_NUM && i < working_set->working_set_size; i++){
        int flag = 0;
        for (int j = 0; j < loaded_cnt; j++){
            if (working_set->loaded_pages[j] == working_set->access_page_ids[i]){
                flag = 1;
                break;
            }
        }

        if (!flag){
            working_set->loaded_pages[loaded_cnt++] = working_set->access_page_ids[i];
            enqueue(&queue, loaded_cnt-1);
        }
    }

    for (int i = 0; i < working_set->access_length; i++){
        if (working_set->access_modes[i] == ACCESS_READ)
            fprintf(stdout, "访问方式：读\t");
        else if (working_set->access_modes[i] == ACCESS_WRITE)
            fprintf(stdout, "访问方式：写\t");
        else
            fprintf(stderr, "错误：未知的访问方式!\n");

        fprintf(stdout, "页号：%4d\t物理块号：%4d\t", working_set->access_page_ids[i], working_set->pages[working_set->access_page_ids[i]].block_id);

        // 预装入阶段
        if (i < PRELOADED_NUM && i < working_set->working_set_size){
            // int flag = 0;
            // for (int j = 0; j < loaded_cnt; j++){
            //     if (working_set->loaded_pages[j] == working_set->access_page_ids[i]){
            //         flag = 1;
            //         break;
            //     }
            // }

            // if (!flag){
            //     working_set->loaded_pages[loaded_cnt++] = working_set->access_page_ids[i];
            //     enqueue(&queue, loaded_cnt-1);
            // }

            working_set->total_access_num++;
            print_loaded_pages(working_set);
            fprintf(stdout, "\t预装入内存\n");
            continue;
        }

        // 页面中断
        int page_index = find_page(working_set, working_set->access_page_ids[i]);
        if (page_index != -1){
            // 页面已装入
            print_loaded_pages(working_set);
            working_set->total_access_num++;
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t页面已在内存\n");
        }
        else{
            // 页面未装入
            start_time = clock();

            int replace_id = 0;
            // 如果工作集未装满
            if(loaded_cnt < working_set->working_set_size){
                replace_id = loaded_cnt;
                loaded_cnt++;
            }
            else{
                replace_id = dequeue(&queue);
            }
            
            // printf("**debug replace_id: %d**", replace_id);
            
            // 替换页面
            working_set->loaded_pages[replace_id] = working_set->access_page_ids[i];
            enqueue(&queue, replace_id);

            working_set->page_fault_num++;
            working_set->total_access_num++;

            end_time = clock();
            total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

            print_loaded_pages(working_set);
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t触发缺页中断\n");
        }
    }

    fprintf(stdout, "缺页中断次数：%d\n", working_set->page_fault_num);
    fprintf(stdout, "缺页率：%.2lf%%\n", (double)(working_set->page_fault_num) / working_set->total_access_num * 100);
    fprintf(stdout, "淘汰页查找时间开销：%lfs\n", total_time);

    // 释放队列
    freeQueue(&queue);
}

// 最近最久未使用页面置换算法（LRU）
void lru_page_replacement(WorkingSet *working_set){
    int loaded_cnt = 0;
    double total_time = 0;

    clock_t start_time, end_time;

    // 预装入阶段
    for (int i = 0;i < PRELOADED_NUM && i < working_set->working_set_size; i++){
        int flag = 0;
        for (int j = 0; j < loaded_cnt; j++){
            if (working_set->loaded_pages[j] == working_set->access_page_ids[i]){
                flag = 1;
                break;
            }
        }

        if (!flag){
            working_set->loaded_pages[loaded_cnt++] = working_set->access_page_ids[i];
        }
    }

    for (int i = 0; i < working_set->access_length; i++){
        if (working_set->access_modes[i] == ACCESS_READ)
            fprintf(stdout, "访问方式：读\t");
        else if (working_set->access_modes[i] == ACCESS_WRITE)
            fprintf(stdout, "访问方式：写\t");
        else
            fprintf(stderr, "错误：未知的访问方式!\n");

        fprintf(stdout, "页号：%4d\t物理块号：%4d\t", working_set->access_page_ids[i], working_set->pages[working_set->access_page_ids[i]].block_id);

        // 预装入阶段
        if (i < PRELOADED_NUM && i < working_set->working_set_size){
            working_set->total_access_num++;
            print_loaded_pages(working_set);
            fprintf(stdout, "\t预装入内存\n");
            continue;
        }

        // 页面中断
        int page_index = find_page(working_set, working_set->access_page_ids[i]);
        if (page_index != -1){
            // 页面已装入
            print_loaded_pages(working_set);
            working_set->total_access_num++;
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t页面已在内存\n");
        }
        else{
            // 页面未装入
            start_time = clock();

            int replace_id = 0;
            // 如果工作集未装满
            if(loaded_cnt < working_set->working_set_size){
                replace_id = loaded_cnt;
                loaded_cnt++;
            }
            else{
                
                int res = 0x3f3f3f3f;
                int tmp;
                // 记录每个页面的在i时刻前的最近访问时刻
                for (int j = 0; j < working_set->working_set_size; j++){
                    for (int k = i-1; k >= 0; k--){
                        tmp = k;
                        if (working_set->access_page_ids[k] == working_set->loaded_pages[j]){    
                            break;
                        }
                    }
                    if (tmp < res){
                        res = tmp;
                        replace_id = j;
                    }
                }
                
            }

            working_set->loaded_pages[replace_id] = working_set->access_page_ids[i];
            working_set->page_fault_num++;
            working_set->total_access_num++;

            end_time = clock();
            total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

            print_loaded_pages(working_set);
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t触发缺页中断\n");
        }
    }

    fprintf(stdout, "缺页中断次数：%d\n", working_set->page_fault_num);
    fprintf(stdout, "缺页率：%.2lf%%\n", (double)(working_set->page_fault_num) / working_set->total_access_num * 100);
    fprintf(stdout, "淘汰页查找时间开销：%lfs\n", total_time);
}


// 简单时钟页面置换算法（Clock）
void simple_clock_page_replacement(WorkingSet *working_set){
    int loaded_cnt = 0;
    double total_time = 0;

    clock_t start_time, end_time;

    // 预装入阶段
    for (int i = 0;i < PRELOADED_NUM && i < working_set->working_set_size; i++){
        int flag = 0;
        for (int j = 0; j < loaded_cnt; j++){
            if (working_set->loaded_pages[j] == working_set->access_page_ids[i]){
                flag = 1;
                break;
            }
        }

        if (!flag){
            working_set->loaded_pages[loaded_cnt++] = working_set->access_page_ids[i];
        }
    }

    int replace_id = 0;

    for (int i = 0; i < working_set->access_length; i++){
        if (working_set->access_modes[i] == ACCESS_READ)
            fprintf(stdout, "访问方式：读\t");
        else if (working_set->access_modes[i] == ACCESS_WRITE)
            fprintf(stdout, "访问方式：写\t");
        else
            fprintf(stderr, "错误：未知的访问方式!\n");

        fprintf(stdout, "页号：%4d\t物理块号：%4d\t", working_set->access_page_ids[i], working_set->pages[working_set->access_page_ids[i]].block_id);

        // 预装入阶段
        if (i < PRELOADED_NUM && i < working_set->working_set_size){
            
            // 表示页面已被访问过
            working_set->pages[working_set->access_page_ids[i]].is_accessed = 1;

            working_set->total_access_num++;
            print_loaded_pages(working_set);
            fprintf(stdout, "\t预装入内存\n");
            continue;
        }

        // 页面中断
        int page_index = find_page(working_set, working_set->access_page_ids[i]);
        if (page_index != -1){
            // 页面已装入
            print_loaded_pages(working_set);
            working_set->total_access_num++;
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t页面已在内存\n");
        }
        else{
            // 页面未装入
            start_time = clock();

            // 如果工作集未装满
            if(loaded_cnt < working_set->working_set_size){
                replace_id = loaded_cnt;
                loaded_cnt++;
            }
            else{
                
                while (1)
                {
                    if (working_set->pages[working_set->loaded_pages[replace_id]].is_accessed == 1){
                        working_set->pages[working_set->loaded_pages[replace_id]].is_accessed = 0;
                        replace_id = (replace_id + 1) % working_set->working_set_size;
                    }
                    else
                    {
                        break;
                    }
                }
                
            }

            working_set->loaded_pages[replace_id] = working_set->access_page_ids[i];
            working_set->page_fault_num++;
            working_set->total_access_num++;
            
            // 表示页面已被访问过
            working_set->pages[working_set->access_page_ids[i]].is_accessed = 1;

            end_time = clock();
            total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

            print_loaded_pages(working_set);
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t触发缺页中断\n");
        }
    }

    fprintf(stdout, "缺页中断次数：%d\n", working_set->page_fault_num);
    fprintf(stdout, "缺页率：%.2lf%%\n", (double)(working_set->page_fault_num) / working_set->total_access_num * 100);
    fprintf(stdout, "淘汰页查找时间开销：%lfs\n", total_time);
}

// 改进时钟页面置换算法（Clock）
void refined_clock_page_replacement(WorkingSet *working_set){
    int loaded_cnt = 0;
    double total_time = 0;

    clock_t start_time, end_time;

    // 预装入阶段
    for (int i = 0;i < PRELOADED_NUM && i < working_set->working_set_size; i++){
        int flag = 0;
        for (int j = 0; j < loaded_cnt; j++){
            if (working_set->loaded_pages[j] == working_set->access_page_ids[i]){
                flag = 1;
                break;
            }
        }

        if (!flag){
            working_set->loaded_pages[loaded_cnt++] = working_set->access_page_ids[i];
        }
    }

    int replace_id = 0;

    for (int i = 0; i < working_set->access_length; i++){
        if (working_set->access_modes[i] == ACCESS_READ)
            fprintf(stdout, "访问方式：读\t");
        else if (working_set->access_modes[i] == ACCESS_WRITE)
            fprintf(stdout, "访问方式：写\t");
        else
            fprintf(stderr, "错误：未知的访问方式!\n");

        fprintf(stdout, "页号：%4d\t物理块号：%4d\t", working_set->access_page_ids[i], working_set->pages[working_set->access_page_ids[i]].block_id);

        // 预装入阶段
        if (i < PRELOADED_NUM && i < working_set->working_set_size){
            
            // 表示页面已被访问过
            working_set->pages[working_set->access_page_ids[i]].is_accessed = 1;
            // 如果是写操作，标记为已修改
            if(working_set->access_modes[i] == ACCESS_WRITE){
                working_set->pages[working_set->access_page_ids[i]].is_modified = 1;
            }

            working_set->total_access_num++;
            print_loaded_pages(working_set);
            fprintf(stdout, "\t预装入内存\n");
            continue;
        }

        // 页面中断
        int page_index = find_page(working_set, working_set->access_page_ids[i]);
        if (page_index != -1){
            // 页面已装入
            print_loaded_pages(working_set);
            working_set->total_access_num++;
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t页面已在内存\n");
        }
        else{
            // 页面未装入
            start_time = clock();

            // 如果工作集未装满
            if(loaded_cnt < working_set->working_set_size){
                replace_id = loaded_cnt;
                loaded_cnt++;
            }
            else{
                int epoch = 4;
                int f = 0;
                for(int k = 1;k<=epoch;k++){
                    for(int j = 0; j < working_set->working_set_size; j++){
                        if(k == 1 || k == 3){
                            if(working_set->pages[working_set->loaded_pages[replace_id]].is_accessed == 0
                            && working_set->pages[working_set->loaded_pages[replace_id]].is_modified == 0){
                                f = 1;
                                break;
                            }
                        }
                        else if(k == 2 || k == 4){
                            if(working_set->pages[working_set->loaded_pages[replace_id]].is_accessed == 0
                            && working_set->pages[working_set->loaded_pages[replace_id]].is_modified == 1){
                                f = 1;
                                break;
                            }
                            else{
                                working_set->pages[working_set->loaded_pages[replace_id]].is_accessed = 0;
                            }
                        }
                        replace_id = (replace_id + 1) % working_set->working_set_size;
                    }
                    if(f == 1){
                        break;
                    }
                }
            }

            working_set->loaded_pages[replace_id] = working_set->access_page_ids[i];
            working_set->page_fault_num++;
            working_set->total_access_num++;
            
            // 表示页面已被访问过
            working_set->pages[working_set->access_page_ids[i]].is_accessed = 1;
            // 如果是写操作，标记为已修改
            if(working_set->access_modes[i] == ACCESS_WRITE){
                working_set->pages[working_set->access_page_ids[i]].is_modified = 1;
            }

            end_time = clock();
            total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

            print_loaded_pages(working_set);
            //double rate = (double)(working_set->page_fault_num) / working_set->total_access_num;
            fprintf(stdout, "\t触发缺页中断\n");
        }
    }

    fprintf(stdout, "缺页中断次数：%d\n", working_set->page_fault_num);
    fprintf(stdout, "缺页率：%.2lf%%\n", (double)(working_set->page_fault_num) / working_set->total_access_num * 100);
    fprintf(stdout, "淘汰页查找时间开销：%lfs\n", total_time);
}

