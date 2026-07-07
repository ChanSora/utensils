#ifndef BENCH_COMMON_H
#define BENCH_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// ---------- 计时 ----------
double get_time(void);

// ---------- 数据生成 ----------
// 生成包含 n 个不重复随机数的数组（范围 0 ~ n*10，种子固定以保证可复现）
int* gen_random_unique(int n, int seed);
// 生成已排序的 0..n-1 数组（测试顺序插入）
int* gen_sorted(int n);
// 生成逆序的 n-1..0 数组（测试最坏情况）
int* gen_reversed(int n);
// 打乱已有数组（用于删除顺序）
void shuffle_array(int* arr, int n, int seed);

// ---------- 结果打印 ----------
// 打印带颜色的分隔标题（便于阅读输出）
void print_title(const char* title);
// 打印单行结果（树名、操作、数据规模、耗时、旋转次数）
void print_result(const char* tree, const char* op, int n, double time, long long rotations);

// ---------- 可选：数据校验 ----------
// 检查数组是否升序（用于验证中序遍历结果）
int is_sorted(int* arr, int n);

#endif