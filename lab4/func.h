#ifndef PAGE_H
#define PAGE_H

// 预装入页面数
#define PRELOADED_NUM 3

// 页面访问模式
#define PAGE_READ_ONLY 0       // 只读
#define PAGE_READ_WRITE 1      // 读写

// 访问模方式常量（读或写）
#define ACCESS_READ 1
#define ACCESS_WRITE 2


// 页面结构体
typedef struct Page {
    int block_id;       // 物理块号
    int access_mode;    // 访问模式（只读或读写）
    int last_access_time;   // 最近访问时刻（用于LRU算法）
    int is_accessed;    // 访问位（0 或 1）
    int is_modified;    // 修改位（0 或 1）
} Page;

// 工作集结构体
typedef struct WorkingSet {

    Page *pages;  // 页表
    int total_page_num;     // 页面总数
    
    int *loaded_pages;  // 已加载的页面
    int working_set_size;   // 工作集可容纳的页面数
    int loaded_page_num;    // 已加载的页面数
    int start_page_id;   // 起始页面ID
    int speed;  // 移动的速率

    int page_fault_num;           // 缺页次数
    int total_access_num;       // 总访问次数

    int access_length;          // 访问序列的长度
    int *access_page_ids;       // 访问页面的ID数组
    int *access_modes;   // 访问模式的数组（读/写）

} WorkingSet;

// 初始化工作集
void initialize_working_set(WorkingSet *working_set, int working_set_size, int start_page_id, int total_page_num, int speed, int access_length);

// 释放工作集
void free_working_set(WorkingSet *working_set);

// 根据工作集生成访问序列
void generate_access_sequence(WorkingSet *working_set);

// 打印已加载的页面
void print_loaded_pages(WorkingSet *working_set);

// 检查页面是否已经在内存中
int match_page(WorkingSet *working_set, int page_id);

// 页面置换算法
void optimal_page_replacement(WorkingSet *working_set);           // 最佳页面置换算法
void fifo_page_replacement(WorkingSet *working_set);              // 先进先出页面置换算法
void lru_page_replacement(WorkingSet *working_set);               // 最近最久未使用页面置换算法
void simple_clock_page_replacement(WorkingSet *working_set);      // 简单Clock页面置换算法
void refined_clock_page_replacement(WorkingSet *working_set);     // 改进型Clock页面置换算法

#endif