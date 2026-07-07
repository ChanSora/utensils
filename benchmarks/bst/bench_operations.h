#ifndef BENCH_OPERATIONS_H
#define BENCH_OPERATIONS_H

typedef struct {
    void* (*create)(int capacity);
    void (*destroy)(void* tree);
    void (*insert)(void* tree, int val);
    void (*erase)(void* tree, int val);
    int (*find)(void* tree, int val);
} TreeOps;

// 测试用例：顺序读写（返回耗时，单位秒）
double run_sequential_insert(void* tree, TreeOps ops, int* data, int n);
double run_sequential_erase(void* tree, TreeOps ops, int* data, int n);

// 测试用例：随机读写
double run_random_insert(void* tree, TreeOps ops, int* data, int n);
double run_random_find(void* tree, TreeOps ops, int* data, int n);
double run_random_erase(void* tree, TreeOps ops, int* data, int n);

// 测试用例：混合负载（50%插入 + 50%查找）
double run_mixed_workload(void* tree, TreeOps ops, int* data, int n);
double run_concurrent_find(void* tree, TreeOps ops, int* data, int n, int num_threads);
#endif