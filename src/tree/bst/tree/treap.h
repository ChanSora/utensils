/**
 * @file treap.h
 * @brief Treap (Tree + Heap)
 *
 * A randomized BST where each node carries a random priority in addition
 * to its key value. It satisfies both BST ordering and heap ordering
 * (parent priority > child priority), maintaining approximate balance.
 *
 * On insert, rotations restore the heap property (like bubble-up).
 * On erase, the target is rotated down to a leaf then removed.
 * With uniformly random priorities, expected height is O(log n).
 *
 * Implementation features:
 * - Uses PCG64 RNG for priority generation
 * - Sentinel node (nil) simplifies boundary handling
 * - Pre-allocated node pool
 * - Subtree size enables order-based queries
 */
#ifndef TREAP_H
#define TREAP_H

#include <utensils/rng.h>

typedef struct Node Node;

/** Treap main structure */
typedef struct {
    Node* root;       ///< Root node (initially nil)
    Node* nil;        ///< Sentinel node
    Node* nodes;      ///< Pre-allocated node pool
    int idx;          ///< Number of nodes used
    int capacity;     ///< Maximum node pool capacity
    RNG* rng;         ///< Random number generator (for node priorities)
} Treap;

/**
 * @brief Create a Treap
 * @param n Maximum capacity
 * @return Pointer to the tree on success, NULL on failure
 */
Treap* treap_create(int n);

/** @brief Insert a value */
void treap_insert(Treap* tree, int val);

/** @brief Erase a value */
void treap_erase(Treap* tree, int val);

/** @brief Find a node by value (returns nil if not found) */
Node* treap_find(Treap* tree, int val);

/** @brief Find a value and return it */
int treap_find_val(Treap* tree, int val);

/** @brief Destroy the treap */
void treap_destroy(Treap* tree);

/** @brief Get first node > val (upper_bound) */
Node* treap_upper_bound(Treap* tree, int val);

/** @brief Get first node >= val (lower_bound) */
Node* treap_lower_bound(Treap* tree, int val);

/** @brief Get successor of val */
Node* treap_next(Treap* tree, int val);

/** @brief Get successor value of val */
int treap_next_val(Treap* tree, int val);

/** @brief Get predecessor of val */
Node* treap_prev(Treap* tree, int val);

/** @brief Get predecessor value of val */
int treap_prev_val(Treap* tree, int val);

/** @brief Get node with rank k (1-indexed) */
Node* treap_kth(Treap* tree, int k);

/** @brief Get value with rank k (1-indexed) */
int treap_kth_val(Treap* tree, int k);

/** @brief Get rank of val (1-indexed) */
int treap_rank(Treap* tree, int val);

#endif