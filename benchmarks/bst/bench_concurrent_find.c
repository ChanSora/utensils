#include <pthread.h>
#include "bench_common.h"
#include "bench_operations.h"

// 线程参数结构体
typedef struct {
    void* tree;
    TreeOps ops;
    int* keys;          // 要查找的 key 数组（每个线程查不同的片段）
    int start;
    int end;
    long long found_count;  // 找到的次数（用于校验）
} ThreadArgs;

static void* thread_find_worker(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    long long local_found = 0;
    for (int i = args->start; i < args->end; i++) {
        // 完全不用加锁，直接读
        if (args->ops.find(args->tree, args->keys[i]) != 0) {
            local_found++;
        }
    }
    args->found_count = local_found;
    return NULL;
}

// 测试函数：num_threads 并发查找 n 次
double run_concurrent_find(void* tree, TreeOps ops, int* data, int n, int num_threads) {
    // 1. 预先填充树（不计时）
    for (int i = 0; i < n; i++) ops.insert(tree, data[i]);

    // 2. 准备线程数据（把 n 个 key 平分给各线程）
    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];
    int chunk = n / num_threads;

    double start = get_time();
    for (int t = 0; t < num_threads; t++) {
        args[t].tree = tree;
        args[t].ops = ops;
        args[t].keys = data;
        args[t].start = t * chunk;
        args[t].end = (t == num_threads - 1) ? n : (t + 1) * chunk;
        args[t].found_count = 0;
        pthread_create(&threads[t], NULL, thread_find_worker, &args[t]);
    }

    // 3. 等待所有线程结束
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }
    double end = get_time();

    // 4. 校验：所有线程找到的总数应该等于 n（因为查的都是存在的 key）
    long long total_found = 0;
    for (int t = 0; t < num_threads; t++) total_found += args[t].found_count;
    printf("Total found: %lld (Expected: %d)\n", total_found, n);

    return end - start;
}