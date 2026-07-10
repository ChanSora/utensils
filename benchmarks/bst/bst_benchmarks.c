/*
 * ============================================================================
 * Benchmark Suite for AVL vs Red‑Black Tree (Container‑Style Implementation)
 * ============================================================================
 *
 * Description:
 *   This program benchmarks the performance of two balanced binary search trees:
 *     - AVL Tree (strictly balanced)
 *     - Red‑Black Tree (approximately balanced)
 *
 *   The tests measure execution time under various workloads:
 *     - Sequential insert / erase (sorted data)
 *     - Random insert / find / erase (random data)
 *     - Mixed workload (50% insert + 50% find)
 *
 *   All trees share the same underlying memory pool (pre‑allocated node arrays)
 *   to ensure fair comparison and maximise cache locality.
 *
 * Usage:
 *   ./bench_all [-N <size>] [-h]
 *
 *   Options:
 *     -N <size>   Number of elements to insert/operate on (default: 100000)
 *     -h          Show this help message
 *
 * Example:
 *   ./bench_all -N 1000000
 *
 * Output:
 *   For each test, the program prints:
 *     - Tree type (AVL / RB / Treap / Splay)
 *     - Operation name
 *     - Wall‑clock time in seconds
 *     - (Optional) Rotation counts if exposed via TreeOps
 *
 * Design Notes:
 *   - All test data is pre‑generated using a fixed random seed (42) to ensure
 *     reproducibility and identical input sequences for both tree types.
 *   - The `TreeOps` structure abstracts tree‑specific functions, allowing the
 *     same benchmark logic to be reused for AVL and RB without duplication.
 *   - Timing uses monotonic clock (`get_time()`) to avoid system time skew.
 *
 * Author:  ChanSora
 * Date:    7/7/2026
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bench/bench_common.h"
#include "bench/bench_operations.h"

#include <utensils/tree.h>

#define NUM_TREES 4
#define NUM_BENCHMARKS 6

static void print_usage(const char* prog) {
    printf("Usage: %s [-N size] [-h]\n", prog);
    printf("  -N size   : number of elements for benchmark (default: 100000)\n");
    printf("  -h        : show this help message\n");
}

struct bench_op {
    const char* benchmark_name;
    double (*func)(Tree* tree, int* data, int n);
};

struct bench_op benchs[NUM_BENCHMARKS] = {
    {"Sequential Insert", run_sequential_insert},
    {"Sequential Erase", run_sequential_erase},
    {"Random Insert", run_random_insert},
    {"Random Find", run_random_find},
    {"Random Erase", run_random_erase},
    {"Mixed Workload", run_mixed_workload}
};

TreeType types[NUM_TREES] = {
        AVL_TREE, RB_TREE, TREAP, SPLAY
};

const char* names[NUM_TREES] = {
    "AVL", "RB", "Treap", "Splay"
};

int main(int argc, char** argv) {
    int N = 1000000;

    int opt;
    while ((opt = getopt(argc, argv, "N:h")) != -1) {
        switch (opt) {
            case 'N':
                N = atoi(optarg);
                if (N <= 0) {
                    fprintf(stderr, "Error: N must be a positive integer.\n");
                    return 1;
                }
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    printf("Running benchmarks with N = %d\n", N);

    int* data = gen_sorted(N);

    double t;

    for (int i = 0; i < NUM_BENCHMARKS; i++) {
        printf("=== %s ===\n", benchs[i].benchmark_name);
        for (int j = 0; j < NUM_TREES; j++) {
            Tree tree = tree_init(types[j]);
            tree.create(&tree, N);
            t = benchs[i].func(&tree, data, N);
            printf("[%s]: %.4f sec\n", names[j], t);
        }
    }

    return 0;
}