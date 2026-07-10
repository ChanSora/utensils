#ifndef UTENSILS_RNG_H
#define UTENSILS_RNG_H

#include <stdint.h>

// ---------- RNG 结构体（每个实例独立，线程安全） ----------
typedef struct {
    uint64_t state;
} RNG;

// ---------- 生命周期 ----------
// 使用种子初始化 RNG（种子 0 会被自动替换为随机值）
void rng_init(RNG* rng, uint64_t seed);

// 使用当前时间 + 指针地址作为种子（方便快速使用）
void rng_init_auto(RNG* rng);

// ---------- 核心随机数生成 ----------
// 返回 [0, 2^64) 范围内的 64 位无符号整数
uint64_t rng_next_u64(RNG* rng);

uint32_t rng_next_u32(RNG* rng);

// 返回 [0.0, 1.0) 范围内的双精度浮点数
double rng_next_double(RNG* rng);

// 返回 [0.0, 1.0] 范围内的双精度浮点数（包含 1.0）
double rng_next_double_inclusive(RNG* rng);

// ---------- 跳表 / Treap 专用 ----------
// 生成跳表的随机层高（几何分布，p 为向上概率，常见 p=0.25）
int rng_random_level(RNG* rng, int max_level, double p);

#endif // UTENSILS_RNG_H