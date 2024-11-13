#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "scheduling.h"

int main() {
    printf("请输入输入文件的文件路径（例如：input_fcfs.txt）：");
    char input_file_path[100];
    scanf("%s", input_file_path);

    FILE *input_file = fopen(input_file_path, "r");
    if (input_file == NULL) {
        printf("错误：无法打开输入文件。\n");
        return 1;
    }

    // 生成对应的输出文件名
    char output_file_path[100];
    strcpy(output_file_path, input_file_path);
    char *dot = strrchr(output_file_path, '.');
    if (dot != NULL) {
        strcpy(dot, "_output.txt");
    } else {
        strcat(output_file_path, "_output.txt");
    }

    FILE *output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        printf("错误：无法打开输出文件。\n");
        fclose(input_file);
        return 1;
    }

    // 从输入文件中读取调度类型
    int scheduling_type;
    fscanf(input_file, "%d", &scheduling_type);

    // 根据调度类型执行相应的调度算法
    switch (scheduling_type) {
        case 0:
            fcfs_scheduling(input_file, output_file);
            break;
        case 1:
            sjf_scheduling(input_file, output_file);
            break;
        case 2:
            psa_scheduling(input_file, output_file);
            break;
        case 3:
            rr_scheduling(input_file, output_file);
            break;
        default:
            fprintf(output_file, "无效输入。退出程序。\n");
            break;
    }

    printf("输出已写入 %s\n", output_file_path);

    // 关闭文件
    fclose(input_file);
    fclose(output_file);

    return 0;
}
