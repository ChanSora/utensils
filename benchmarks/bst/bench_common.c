#include "bench_common.h"

// ---------- 计时 ----------
double get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// ---------- 数据生成 ----------
// 生成不重复随机数（用简单的标记数组去重，适合 n 不太大的场景）
int* gen_random_unique(int n, int seed) {
    srand(seed);
    int* arr = malloc(n * sizeof(int));
    int* mark = calloc(n * 10 + 1, sizeof(int)); // 假设范围 0 ~ n*10
    
    int cnt = 0;
    while (cnt < n) {
        int val = rand() % (n * 10);
        if (!mark[val]) {
            mark[val] = 1;
            arr[cnt++] = val;
        }
    }
    free(mark);
    return arr;
}

int* gen_sorted(int n) {
    int* arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = i;
    return arr;
}

int* gen_reversed(int n) {
    int* arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = n - 1 - i;
    return arr;
}

void shuffle_array(int* arr, int n, int seed) {
    srand(seed);
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

// ---------- 结果打印 ----------
void print_title(const char* title) {
    printf("\n========== %s ==========\n", title);
}

void print_result(const char* tree, const char* op, int n, double time, long long rotations) {
    printf("%-8s | %-15s | N=%-8d | Time=%-10.6f | Rotations=%-8lld\n",
           tree, op, n, time, rotations);
}

// ---------- 校验 ----------
int is_sorted(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i-1]) return 0;
    }
    return 1;
}