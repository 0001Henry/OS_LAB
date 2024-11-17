#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main()
{
    srand(22281067); // 设置随机种子，使结果可复现
    fprintf(stdout, "欢迎来到页面淘汰算法模拟实现系统！\n");
    fprintf(stdout, "作者：衡勇睿\t学号：22281067\n");

    while (1)
    {
        WorkingSet working_set; // 工作集
        int user_choice;
        fprintf(stdout, "*****菜单*****\n");
        fprintf(stdout, "  1. 使用置入的访问序列\n");
        fprintf(stdout, "  2. 随机生成访问序列\n");
        fprintf(stdout, "  3. 退出\n");
        fprintf(stdout, "请输入选择的功能编号: ");
        fscanf(stdin, "%d", &user_choice);

        int working_set_size, start_page_id, total_page_num, speed, access_length;
        switch (user_choice)
        {
        case 1:
        {
            // 配置文件路径
            char config_file_path[100]="config1.txt";
            // 读取配置文件
            FILE *config_file = fopen(config_file_path, "r");
            if (config_file == NULL)
            {
                fprintf(stderr, "错误: 无法打开配置文件\n");
                return 0;
            }
            fscanf(config_file, "%d %d %d %d %d", &total_page_num, &access_length, &working_set_size, &speed, &start_page_id);

            initialize_working_set(&working_set, working_set_size, start_page_id, total_page_num, speed, access_length);

            for (int i = 0; i < access_length; i++)
            {
                fscanf(config_file, "%d ", &working_set.access_page_ids[i]);
                // 随机选择访问模式
                double r = (double)rand() / RAND_MAX;
                if (r > 0.7 && working_set.pages[working_set.access_page_ids[i]].access_mode == PAGE_READ_WRITE)
                    working_set.access_modes[i] = ACCESS_WRITE;
                else
                    working_set.access_modes[i] = ACCESS_READ;
            }
            fclose(config_file);
            break;
        }
        case 2:
        {
            // start_page_id = 0;
            // 配置文件路径
            char config_file_path[100]="config2.txt";
            // 读取配置文件
            FILE *config_file = fopen(config_file_path, "r");
            if (config_file == NULL)
            {
                fprintf(stderr, "错误: 无法打开配置文件\n");
                return 0;
            }
            fscanf(config_file, "%d %d %d %d %d", &total_page_num, &access_length, &working_set_size, &speed, &start_page_id);

            initialize_working_set(&working_set, working_set_size, start_page_id, total_page_num, speed, access_length);
            generate_access_sequence(&working_set);

            fclose(config_file);
            break;
        }
        case 3:
            fprintf(stdout, "再见！\n");
            return 0;
        default:
            fprintf(stderr, "错误: 未知的功能编号\n");
            return 0;
        }

        // 打印生成的访问序列
        fprintf(stdout, "访问序列:\n");
        for (int i = 0; i < working_set.access_length; i++)
        {
                fprintf(stdout, "%d  ", working_set.access_page_ids[i]);
        }
        fprintf(stdout, "\n");

        // 提示用户选择页面置换算法
        fprintf(stdout, "请选择一个页面置换算法:\n");
        fprintf(stdout, "  1. 最优页面置换\n");
        fprintf(stdout, "  2. FIFO（先进先出）页面置换\n");
        fprintf(stdout, "  3. LRU（最近最少使用）页面置换\n");
        fprintf(stdout, "  4. 简单时钟页面置换\n");
        fprintf(stdout, "  5. 改进时钟页面置换\n");
        // fprintf(stdout, "  6. 全部执行\n");
        fprintf(stdout, "请输入使用的页面置换算法的编号: ");
        fscanf(stdin, "%d", &user_choice);

        // 执行用户选择的页面置换算法
        switch (user_choice)
        {
        case 1:
            optimal_page_replacement(&working_set);
            break;
        case 2:
            fifo_page_replacement(&working_set);
            break;
        case 3:
            lru_page_replacement(&working_set);
            break;
        case 4:
            simple_clock_page_replacement(&working_set);
            break;
        case 5:
            refined_clock_page_replacement(&working_set);
            break;
        // case 6:
        // {
        //     // 拷贝工作集
        //     WorkingSet working_set_2;

        //     optimal_page_replacement(&working_set);

        //     fifo_page_replacement(&working_set_2);
        //     free_working_set(&working_set_2);

        //     lru_page_replacement(&working_set_3);
        //     free_working_set(&working_set_3);

        //     simple_clock_page_replacement(&working_set_4);
        //     free_working_set(&working_set_4);

        //     refined_clock_page_replacement(&working_set_5);
        //     free_working_set(&working_set_5);
        //     break;
        // }
        default:
            fprintf(stderr, "错误: 未知的页面置换算法\n");
            break;
        }

        // 释放工作集
        free_working_set(&working_set);
    }
    return 0;
}
