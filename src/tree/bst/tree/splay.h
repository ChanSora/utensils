/**
 * @file splay.h
 * @brief Splay Tree
 *
 * A self-adjusting BST where recently accessed elements are splayed to the root.
 * Every insert, delete, and find splays the accessed node, keeping frequently
 * used nodes near the root for amortized O(log n) performance.
 *
 * Three splay rotation patterns:
 *   zig:      single rotation when parent is the root
 *   zig-zig:  double rotation when node, parent, grandparent are aligned
 *   zig-zag:  double rotation when node, parent, grandparent are opposite
 *
 * Implementation features:
 * - Sentinel node (nil) simplifies boundary checks
 * - Pre-allocated node pool
 * - Subtree size (size) enables order-based queries
 */
#ifndef SPLAY_H
#define SPLAY_H

typedef struct Node Node;

/** Splay tree main structure */
typedef struct {
    Node* root;       ///< Root node (initially nil)
    Node* nil;        ///< Sentinel node
    Node* nodes;      ///< Pre-allocated node pool
    int idx;          ///< Number of nodes used
    int capacity;     ///< Maximum node pool capacity
} Splay;

/**
 * @brief Create a splay tree
 * @param n Maximum capacity
 * @return Pointer to the tree on success, NULL on failure
 */
Splay* splay_create(int n);

/** @brief Insert a value */
void splay_insert(Splay* tree, int val);

/** @brief Erase a value */
void splay_erase(Splay* tree, int val);

/** @brief Find a node by value (returns nil if not found) */
Node* splay_find(Splay* tree, int val);

/** @brief Find a value and return it */
int splay_find_val(Splay* tree, int val);

/** @brief Destroy the tree */
void splay_destroy(Splay* tree);

/** @brief Get first node > val */
Node* splay_upper_bound(Splay* tree, int val);

/** @brief Get first node >= val */
Node* splay_lower_bound(Splay* tree, int val);

/** @brief Get successor of val */
Node* splay_next(Splay* tree, int val);

/** @brief Get successor value of val */
int splay_next_val(Splay* tree, int val);

/** @brief Get predecessor of val */
Node* splay_prev(Splay* tree, int val);

/** @brief Get predecessor value of val */
int splay_prev_val(Splay* tree, int val);

/** @brief Get node with rank k (1-indexed) */
Node* splay_kth(Splay* tree, int k);

/** @brief Get value with rank k (1-indexed) */
int splay_kth_val(Splay* tree, int k);

/** @brief Get rank of val (1-indexed) */
int splay_rank(Splay* tree, int val);

#endif