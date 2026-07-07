#include "bench_operations.h"
#include "bench_common.h"
#include <stdlib.h>
#include <string.h>

// ---------- 随机插入 ----------
// 假设 data 包含 n 个不重复的整数，依次插入空树
double run_random_insert(void* tree, TreeOps ops, int* data, int n) {
    double start = get_time();
    for (int i = 0; i < n; i++) {
        ops.insert(tree, data[i]);
    }
    double end = get_time();
    return end - start;
}

// ---------- 随机查找 ----------
// 先插入 data 全部元素（不计时），然后随机查找 n 次（一半找存在的，一半找不存在的）
double run_random_find(void* tree, TreeOps ops, int* data, int n) {
    // 1. 先插入所有数据（不计入查找时间）
    for (int i = 0; i < n; i++) {
        ops.insert(tree, data[i]);
    }

    // 2. 准备查找用的 key 列表：一半来自 data（存在），一半来自 data + 偏移（不存在）
    int* keys = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            keys[i] = data[i];                // 存在的 key
        } else {
            keys[i] = data[i] + 1000000;      // 足够大，保证不存在
        }
    }
    // 打乱 keys 顺序，使查找顺序随机
    shuffle_array(keys, n, 12345);

    // 3. 计时查找
    double start = get_time();
    for (int i = 0; i < n; i++) {
        ops.find(tree, keys[i]);
    }
    double end = get_time();

    free(keys);
    return end - start;
}

// ---------- 随机删除 ----------
// 先插入 data 全部元素（不计时），然后随机打乱删除顺序，计时删除所有元素
double run_random_erase(void* tree, TreeOps ops, int* data, int n) {
    // 1. 插入所有数据
    for (int i = 0; i < n; i++) {
        ops.insert(tree, data[i]);
    }

    // 2. 复制一份 data 并打乱顺序，作为删除顺序
    int* del_order = malloc(n * sizeof(int));
    memcpy(del_order, data, n * sizeof(int));
    shuffle_array(del_order, n, 67890);

    // 3. 计时删除
    double start = get_time();
    for (int i = 0; i < n; i++) {
        ops.erase(tree, del_order[i]);
    }
    double end = get_time();

    free(del_order);
    return end - start;
}

// ---------- 混合负载（50% 插入 + 50% 查找） ----------
// 策略：
//   - 用 data 的前 n/2 个元素构建初始树（不计时）
//   - 然后执行 n 次操作：若随机数 < 0.5，则插入 data 中尚未插入的元素（从 n/2 开始取）；
//     否则查找一个已存在的元素（从已插入的 data[0..n/2-1] 中随机取）
//   - 保证插入的都是新 key，查找的都是存在的 key。
double run_mixed_workload(void* tree, TreeOps ops, int* data, int n) {
    int half = n / 2;
    if (half < 1) half = 1;

    // 1. 插入前 half 个元素作为初始树（不计时）
    for (int i = 0; i < half; i++) {
        ops.insert(tree, data[i]);
    }

    // 2. 准备后续插入的数据（从 half 开始）
    int insert_idx = half;      // 下一个要插入的数据在 data 中的位置
    int max_insert = n;         // 最多插入到 data[n-1]

    // 用于查找的随机索引范围：0 ~ half-1
    // 执行 n 次操作
    double start = get_time();
    for (int i = 0; i < n; i++) {
        if ((rand() % 2) == 0) {
            // 插入操作：如果还有未插入的数据
            if (insert_idx < max_insert) {
                ops.insert(tree, data[insert_idx++]);
            } else {
                // 数据用完了，就查找（避免插入失败）
                int key = data[rand() % half];
                ops.find(tree, key);
            }
        } else {
            // 查找操作：从已插入的前 half 个中随机选一个
            int key = data[rand() % half];
            ops.find(tree, key);
        }
    }
    double end = get_time();

    return end - start;
}