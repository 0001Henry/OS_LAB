# pragma once

#define DISK_SIZE 200

void get_sequence(int seq[], int length);
int cmp(const void *a, const void *b);

int fcfs(int seq[], int length, int* pos);
int sstf(int seq[], int length, int* pos);
int scan(int seq[], int length, int* pos);
int cscan(int seq[], int length, int* pos);
int fscan(int seq[], int length, int* pos);
