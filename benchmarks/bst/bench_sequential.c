#include "bench_common.h"
#include "bench_operations.h"

double run_sequential_insert(void* tree, TreeOps ops, int* data, int n) {
    double start = get_time();
    for (int i = 0; i < n; i++) {
        ops.insert(tree, data[i]);
    }
    double end = get_time();
    return end - start;
}

double run_sequential_erase(void* tree, TreeOps ops, int* data, int n) {
    double start = get_time();
    for (int i = 0; i < n; i++) {
        ops.erase(tree, data[i]);
    }
    double end = get_time();
    return end - start;
}