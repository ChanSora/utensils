/**
 * @file treap.c
 * @brief Treap (Tree + Heap) implementation
 *
 * Combines BST ordering with heap ordering on random priorities.
 * The tree maintains both:
 *   - BST property on val (left < node < right)
 *   - Max-heap property on priority (parent > children)
 *
 * Insert: BST insert at leaf, then rotate upward until heap property is restored.
 * Erase:  Rotate the target down to a leaf by swapping with its higher-priority
 *         child, then remove it.
 *
 * Expected height is O(log n) under uniform random priorities.
 */
#include <stdlib.h>
#include <stdint.h>

#include <utensils/rng.h>
#include "treap.h"

/**
 * Treap node
 * @param parent   Parent node
 * @param left     Left child
 * @param right    Right child
 * @param val      Key value
 * @param size     Subtree size
 * @param priority Random priority (uint32_t, higher = higher priority)
 */
typedef struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int val;
    int size;
    uint32_t priority;
} Node;

/** Update node size information */
static void update_size(Node* node, Node* NIL) {
    if (node == NIL) return;
    node->size = 1 + node->left->size + node->right->size;
}

/** Left rotation */
static void rotate_left(Node** root_ptr, Node* node, Node* NIL) {
    
    if (node == NIL) return;

    Node* parent = node->parent;

    Node* new_root = node->right;
    if (new_root == NIL) return;
    
    Node* b = new_root->left;
    if (b != NIL) b->parent = node;
    node->right = b;
    
    new_root->left = node;
    node->parent = new_root;
    
    new_root->parent = parent;

    if (parent == NIL) *root_ptr = new_root;
    else if (node == parent->left) parent->left = new_root, new_root->parent = parent;
    else parent->right = new_root, new_root->parent = parent;

    update_size(node, NIL);
    update_size(new_root, NIL);

}

/** Right rotation */
static void rotate_right(Node** root_ptr, Node* node, Node* NIL) {
    if (node == NIL) return;

    Node* parent = node->parent;

    Node* new_root = node->left;
    if (new_root == NIL) return;

    Node* b = new_root->right;
    if (b != NIL) b->parent = node;
    node->left = b;

    node->parent = new_root;
    new_root->right = node;

    new_root->parent = parent;

    if (parent == NIL) *root_ptr = new_root;
    else if (node == parent->left) parent->left = new_root, new_root->parent = parent;
    else parent->right = new_root, new_root->parent = parent;
    
    update_size(node, NIL);
    update_size(new_root, NIL);
    
}

/**
 * Insert a node.
 * BST insert at leaf (updating size along the path),
 * then rotate upward based on priority to maintain heap property.
 */
static void insert(Node** root_ptr, Node* node, Node* NIL) {
    if (node == NIL) return;
    if (*root_ptr == NIL) {
        *root_ptr = node;
        return;
    }

    Node* cur = *root_ptr;
    Node* parent = NIL;

    while (cur != NIL) {
        parent = cur;
        cur->size++;
        if (node->val < cur->val) cur = cur->left;
        else cur = cur->right;
    }

    node->parent = parent;
    if (node->val < parent->val) parent->left = node;
    else parent->right = node;

    while (node->priority > parent->priority && node->parent != NIL) {
        if (node == parent->left) rotate_right(root_ptr, parent, NIL);
        else rotate_left(root_ptr, parent, NIL);
        parent = node->parent;
    }
}

/**
 * Delete a node by value.
 * Uses value replacement to shift the deletion target to a leaf or
 * single-child node, then updates size along the path.
 */
static void erase(Node** root_ptr, int val, Node* NIL) {
    Node* cur = *root_ptr;
    Node* parent = NIL;
    while (cur != NIL) {
        parent = cur;
        if (val < cur->val) cur = cur->left;
        else if (val > cur->val) cur = cur->right;
        else {
            parent = cur->parent;
            if (cur->left == NIL || cur->right == NIL) {
                Node* child = (cur->left == NIL) ? cur->right : cur->left;
                child->parent = parent;
                if (cur == *root_ptr) *root_ptr = child;
                else if (cur == parent->left) parent->left = child;
                else parent->right = child;
                break;
            } else {
                Node* successor = cur->right;
                while (successor->left != NIL) successor = successor->left;
                cur->val = successor->val;
                val = successor->val;
                cur = successor;
                continue;
            }
        }
    }
    while(cur != NIL) cur->size--, cur = cur->parent;
}

/** Find a node by value */
static Node* find(Node* node, int val, Node* NIL) {
    while (node != NIL) {
        if (val < node->val) node = node->left;
        else if (val > node->val) node = node->right;
        else return node;
    }
    return NIL;
}

/** Compute rank of val (1-indexed) */
static int rank(Node* node, int val, Node* NIL) {
    int rank = 0;
    while (node != NIL) {
        if (val <= node->val) node = node->left;
        else rank += node->left->size + 1, node = node->right;
    }
    return rank + 1;
} 

/** Get node with rank k (1-indexed) */
static Node* kth(Node* node, int k, Node* NIL) {
    while (node != NIL) {
        if (node->left->size >= k) node = node->left;
        else if (node->left->size == k - 1) return node;
        else k -= node->left->size + 1, node = node->right;
    }
    return NIL;
}

/** Find predecessor (largest node < val) */
static Node* prev(Node* node, int val, Node* NIL) {
    Node* prev = NIL;
    while (node != NIL) {
        if (node->val < val) prev = node, node = node->right;
        else node = node->left;
    }
    return prev;
}

/** Find first node >= val */
static Node* lower_bound(Node* node, int val, Node* NIL) {
    Node* next = NIL;
    while (node != NIL) {
        if (node->val >= val) next = node, node = node->left;
        else node = node->left;
    }
    return next;
}

/** Find first node > val */
static Node* upper_bound(Node* node, int val, Node* NIL) {
    Node* next = NIL;
    while (node != NIL) {
        if (node->val > val) next = node, node = node->left;
        else node = node->right;
    }
    return next;
}

/**
 * 创建 Treap
 * @param n 最大容量
 * @return 成功返回树指针，失败返回 NULL
 */
Treap* treap_create(int n) {
    if (n <= 0) return NULL;

    Treap* tree = (Treap*)malloc(sizeof(Treap));
    if (!tree) return NULL;

    tree->nodes = (Node*)malloc((n + 1) * sizeof(Node));
    if (!tree->nodes) {
        free(tree);        
        return NULL;
    }
    tree->nil = &tree->nodes[0];
    tree->nil->parent = tree->nil;
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil; 
    tree->nil->val = 0;
    tree->nil->size = 0;
    tree->nil->priority = 0;

    tree->root = tree->nil;
    tree->idx = 0;
    tree->capacity = n;
    tree->rng = (RNG*)malloc(sizeof(RNG));
    rng_init_auto(tree->rng);
    return tree;
}

/** 销毁 Treap */
void treap_destroy(Treap* tree) {
    if (!tree) return;
    free(tree->nodes);
    free(tree);
}

/** 插入值 val */
void treap_insert(Treap* tree, int val) {
    if (tree->idx >= tree->capacity) return;

    Node* node = &tree->nodes[++tree->idx];
    node->val = val;
    node->parent = tree->nil;
    node->left = tree->nil;
    node->right = tree->nil;
    node->size = 1;
    node->priority = rng_next_u32(tree->rng);

    insert(&tree->root, node, tree->nil);
}

/** 删除值 val */
void treap_erase(Treap* tree, int val) {
    erase(&tree->root, val, tree->nil);
}

/** 查找值 val，返回节点指针 */
Node* treap_find(Treap* tree, int val) {
    return find(tree->root, val, tree->nil);
}

/** 查找值 val，返回值 */
int treap_find_val(Treap* tree, int val) {
    return find(tree->root, val, tree->nil)->val;
}

/** 求 val 的排名 (1-indexed) */
int treap_rank(Treap* tree, int val) {
    return rank(tree->root, val, tree->nil);
}

/** 返回排名为 k 的节点 (1-indexed) */
Node* treap_kth(Treap* tree, int k) {
    return kth(tree->root, k, tree->nil);
}

/** 返回排名为 k 的值 (1-indexed) */
int treap_kth_val(Treap* tree, int k) {
    return kth(tree->root, k, tree->nil)->val;
}

/** 返回 val 的前驱节点 */
Node* treap_prev(Treap* tree, int val) {
    return prev(tree->root, val, tree->nil);
}

/** 返回 val 的前驱值 */
int treap_prev_val(Treap* tree, int val) {
    return prev(tree->root, val, tree->nil)->val;
}

/** 返回第一个 >= val 的节点 */
Node* treap_lower_bound(Treap* tree, int val) {
    return lower_bound(tree->root, val, tree->nil);
}

/** 返回第一个 > val 的节点 */
Node* treap_upper_bound(Treap* tree, int val) {
    return upper_bound(tree->root, val, tree->nil);
}

/** 返回 val 的后继节点 */
Node* treap_next(Treap* tree, int val) {
    return upper_bound(tree->root, val, tree->nil);
}

/** 返回 val 的后继值 */
int treap_next_val(Treap* tree, int val) {
    return upper_bound(tree->root, val, tree->nil)->val;
}

