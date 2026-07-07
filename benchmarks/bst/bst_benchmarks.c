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
 *     - Tree type (AVL / RB)
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

#include "bench_common.h"
#include "bench_operations.h"

#include "avl_tree.h"
#include "rb_tree.h"

static void print_usage(const char* prog) {
    printf("Usage: %s [-N size] [-h]\n", prog);
    printf("  -N size   : number of elements for benchmark (default: 100000)\n");
    printf("  -h        : show this help message\n");
}

// TreeOps of AVL
static void* avl_create_wrapper(int cap) { return avl_tree_create(cap); }
static void avl_destroy_wrapper(void* t) { avl_tree_destroy((AVL_Tree*)t); }
static void avl_insert_wrapper(void* t, int v) { avl_tree_insert((AVL_Tree*)t, v); }
static void avl_erase_wrapper(void* t, int v) { avl_tree_delete((AVL_Tree*)t, v); }
static int avl_find_wrapper(void* t, int v) { return avl_tree_find_val((AVL_Tree*)t, v); }

// TreeOps of RB
static void* rb_create_wrapper(int cap) { return rb_tree_create(cap); }
static void rb_destroy_wrapper(void* t) { rb_tree_destroy((RB_Tree*)t); }
static void rb_insert_wrapper(void* t, int v) { rb_tree_insert((RB_Tree*)t, v); }
static void rb_erase_wrapper(void* t, int v) { rb_tree_erase((RB_Tree*)t, v); }
static int rb_find_wrapper(void* t, int v) { return rb_tree_find_val((RB_Tree*)t, v); }


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

    int* data = gen_random_unique(N, 42);
    int* sorted_data = gen_sorted(N);

    TreeOps avl_ops = {
        .create = avl_create_wrapper,
        .destroy = avl_destroy_wrapper,
        .insert = avl_insert_wrapper,
        .erase = avl_erase_wrapper,
        .find = avl_find_wrapper
    };

    TreeOps rb_ops = {
        .create = rb_create_wrapper,
        .destroy = rb_destroy_wrapper,
        .insert = rb_insert_wrapper,
        .erase = rb_erase_wrapper,
        .find = rb_find_wrapper
    };

    void *rb, *avl;
    double t;

    printf("=== Sequential Insert ===\n");
    avl = avl_ops.create(N);
    t = run_sequential_insert(avl, avl_ops, sorted_data, N);
    printf("AVL: %.4f sec\n", t);

    rb = rb_ops.create(N);
    t = run_sequential_insert(rb, rb_ops, sorted_data, N);
    printf("RB: %.4f sec\n", t);
    
    printf("=== Sequential Erase ===\n");
    t = run_sequential_erase(avl, avl_ops, sorted_data, N);
    printf("AVL: %.4f sec\n", t);
    avl_ops.destroy(avl);

    t = run_sequential_erase(rb, rb_ops, sorted_data, N);
    printf("RB: %.4f sec\n", t);
    rb_ops.destroy(rb);

    printf("=== Random Insert ===\n");
    avl = avl_ops.create(N);
    t = run_random_insert(avl, avl_ops, data, N);
    printf("AVL: %.4f sec\n", t);
    avl_ops.destroy(avl);

    rb = rb_ops.create(N);
    t = run_random_insert(rb, rb_ops, data, N);
    printf("RB: %.4f sec\n", t);
    rb_ops.destroy(rb);

    printf("=== Random Find ===\n");
    avl = avl_ops.create(N);
    t = run_random_find(avl, avl_ops, data, N);
    printf("AVL: %.4f sec\n", t);
    avl_ops.destroy(avl);

    rb = rb_ops.create(N);
    t = run_random_find(rb, rb_ops, data, N);
    printf("RB: %.4f sec\n", t);
    rb_ops.destroy(rb);

    printf("=== Random Erase ===\n");
    avl = avl_ops.create(N);
    t = run_random_erase(avl, avl_ops, sorted_data, N);
    printf("AVL: %.4f sec\n", t);
    avl_ops.destroy(avl);

    rb = rb_ops.create(N);
    t = run_random_erase(rb, rb_ops, sorted_data, N);
    printf("RB: %.4f sec\n", t);
    rb_ops.destroy(rb);

    printf("=== Mixed Workload ===\n");
    avl = avl_ops.create(N);
    t = run_mixed_workload(avl, avl_ops, data, N);
    printf("AVL: %.4f sec\n", t);
    avl_ops.destroy(avl);

    rb = rb_ops.create(N);
    t = run_mixed_workload(rb, rb_ops, data, N);
    printf("RB: %.4f sec\n", t);
    rb_ops.destroy(rb);

    printf("=== Concurrent Find ===\n");
    avl = avl_ops.create(N);
    t = run_concurrent_find(avl, avl_ops, data, N, 16);
    printf("AVL: %.4f sec\n", t);
    avl_ops.destroy(avl);

    rb = rb_ops.create(N);
    t = run_concurrent_find(rb, rb_ops, data, N, 16);
    printf("RB: %.4f sec\n", t);
    rb_ops.destroy(rb);

    return 0;
}