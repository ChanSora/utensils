/**
 * @file avl_tree.h
 * @brief AVL (Adelson-Velsky and Landis) self-balancing BST
 *
 * An AVL tree maintains the balance factor (left height - right height)
 * within [-1, 0, 1] for every node, guaranteeing O(log n) operations.
 * When an insert or delete causes the balance factor to go out of range,
 * rotations are performed to restore balance.
 *
 * Implementation features:
 * - Sentinel node (nil) simplifies boundary handling
 * - Pre-allocated node pool avoids runtime dynamic allocation
 * - Each node tracks subtree size (size) for order-based queries (kth / rank)
 */
#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct Node Node;

/** AVL tree main structure */
typedef struct {
    Node* root;       ///< Root node (initially nil)
    Node* nil;        ///< Sentinel node (acts as null pointer, simplifies boundary checks)
    Node* nodes;      ///< Pre-allocated node pool
    int idx;          ///< Number of nodes used so far (1-indexed, nodes[0] is sentinel)
    int capacity;     ///< Maximum node pool capacity
} AVL_Tree;

/**
 * @brief Create an AVL tree
 * @param n Maximum capacity (number of nodes)
 * @return Pointer to the tree on success, NULL on failure
 */
AVL_Tree* avl_tree_create(int n);

/** @brief 销毁 AVL 树，释放节点池 */
void avl_tree_destroy(AVL_Tree* tree);

/** @brief Insert a value into the tree */
void avl_tree_insert(AVL_Tree* tree, int val);

/** @brief Erase a value from the tree */
void avl_tree_erase(AVL_Tree* tree, int val);

/** @brief Find a node by value (returns nil if not found) */
Node* avl_tree_find(AVL_Tree* tree, int val);

/** @brief Find a value and return the stored value (ensure find returns non-nil first) */
int avl_tree_find_val(AVL_Tree* tree, int val);

/** @brief Get the rank of val (1-indexed: count of elements < val + 1) */
int avl_tree_rank(AVL_Tree* tree, int val);

/** @brief Get the node with rank k (1-indexed) */
Node* avl_tree_kth(AVL_Tree* tree, int k);

/** @brief Get the value with rank k (1-indexed) */
int avl_tree_kth_val(AVL_Tree* tree, int k);

/** @brief Get the first node >= val (lower_bound) */
Node* avl_tree_lower_bound(AVL_Tree* tree, int val);

/** @brief Get the first node > val (upper_bound) */
Node* avl_tree_upper_bound(AVL_Tree* tree, int val);

/** @brief Get the predecessor of val (largest node < val, nil if none) */
Node* avl_tree_prev(AVL_Tree* tree, int val);

/** @brief Get the predecessor value of val */
int avl_tree_prev_val(AVL_Tree* tree, int val);

/** @brief Get the successor of val (smallest node > val, nil if none) */
Node* avl_tree_next(AVL_Tree* tree, int val);

/** @brief Get the successor value of val */
int avl_tree_next_val(AVL_Tree* tree, int val);

#endif