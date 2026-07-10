/**
 * @file rb_tree.h
 * @brief Red-Black Tree
 *
 * A self-balancing BST that uses node colors (RED/BLACK) and a set of
 * invariants to guarantee O(log n) operations.
 *
 * Red-Black properties (CLRS):
 *   1. Every node is either red or black
 *   2. The root is black
 *   3. Every leaf (nil sentinel) is black
 *   4. Both children of a red node are black
 *   5. Every simple path from a node to its descendant leaves has the
 *      same number of black nodes (same black-height)
 *
 * Implementation features:
 * - Sentinel node (nil) simplifies boundary handling
 * - Pre-allocated node pool avoids dynamic allocation
 * - Subtree size (size) enables order-based queries
 */
#ifndef RB_TREE_H
#define RB_TREE_H

typedef struct Node Node;

/** Red-Black tree main structure */
typedef struct {
    Node* root;       ///< Root node (initially nil)
    Node* nil;        ///< Sentinel node (acts as null, color is BLACK)
    Node* nodes;      ///< Pre-allocated node pool
    int idx;          ///< Number of nodes used (1-indexed, nodes[0] is sentinel)
    int capacity;     ///< Maximum node pool capacity
} RB_Tree;

/**
 * @brief Create a Red-Black tree
 * @param n Maximum capacity
 * @return Pointer to the tree on success, NULL on failure
 */
RB_Tree* rb_tree_create(int n);

/** @brief Insert a value */
void rb_tree_insert(RB_Tree* tree, int val);

/** @brief Erase a value */
void rb_tree_erase(RB_Tree* tree, int val);

/** @brief Find a node by value (returns nil if not found) */
Node* rb_tree_find(RB_Tree* tree, int val);

/** @brief Find a value and return it */
int rb_tree_find_val(RB_Tree* tree, int val);

/** @brief Destroy the tree and free resources */
void rb_tree_destroy(RB_Tree* tree);

/** @brief Get the first node > val (upper_bound) */
Node* rb_tree_upper_bound(RB_Tree* tree, int val);

/** @brief Get the first node >= val (lower_bound) */
Node* rb_tree_lower_bound(RB_Tree* tree, int val);

/** @brief Get the successor of val */
Node* rb_tree_next(RB_Tree* tree, int val);

/** @brief Get the successor value of val */
int rb_tree_next_val(RB_Tree* tree, int val);

/** @brief Get the predecessor of val */
Node* rb_tree_prev(RB_Tree* tree, int val);

/** @brief Get the predecessor value of val */
int rb_tree_prev_val(RB_Tree* tree, int val);

/** @brief Get the node with rank k (1-indexed) */
Node* rb_tree_kth(RB_Tree* tree, int k);

/** @brief Get the value with rank k (1-indexed) */
int rb_tree_kth_val(RB_Tree* tree, int k);

/** @brief Get the rank of val (1-indexed) */
int rb_tree_rank(RB_Tree* tree, int val);

#endif