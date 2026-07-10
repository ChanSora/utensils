/**
 * @file avl_tree.c
 * @brief AVL tree implementation
 *
 * Each node maintains a height value and subtree size.
 * Balance = left->height - right->height, allowed range [-1, 0, 1].
 *
 * Four imbalance cases (checked upward after insert/delete):
 *   LL: pivot left-heavy, pivot->left left-heavy  -> right-rotate pivot
 *   LR: pivot left-heavy, pivot->left right-heavy -> left-rotate pivot->left, then right-rotate pivot
 *   RR: pivot right-heavy, pivot->right right-heavy -> left-rotate pivot
 *   RL: pivot right-heavy, pivot->right left-heavy -> right-rotate pivot->right, then left-rotate pivot
 */
#include "avl_tree.h"
#include <stdlib.h>

inline static int max(int a, int b) {return a > b ? a : b;}

/**
 * AVL tree node
 * @param parent Parent node
 * @param left   Left child
 * @param right  Right child
 * @param val    Node value
 * @param height Subtree height (leaf = 1, nil = 0)
 * @param size   Subtree size (including self, used for kth / rank)
 */
struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int val;
    int height;
    int size;
};

/** Get node height (nil returns 0) */
inline static int get_height(Node* NIL, Node* node) {
    return (node == NIL) ? 0 : node->height;
}

/**
 * Update node height and subtree size.
 * Called after any structural change (insert, delete, rotate) along the path.
 *
 * Height recurrence:
 *   height(node) = 1 + max(height(node->left), height(node->right))
 *   height(NIL) = 0
 */
static void update_height(Node* NIL, Node* node) {
    if (node == NIL) return;
    int left_height = get_height(NIL, node->left);
    int right_height = get_height(NIL, node->right);
    node->height = 1 + max(left_height, right_height);
    node->size = 1 + node->left->size + node->right->size;
}

/**
 * 右旋（LL 情形）
 *
 *        pivot                new_root
 *       /                    /        \
 *    new_root       →       b        pivot
 *    /     \                          /
 *   b       ?                        ?
 *
 * 三步完成：
 *   1. pivot->left = b
 *   2. new_root->right = pivot
 *   3. 将 new_root 接到 pivot 的原父节点上
 *   4. 更新 pivot 和 new_root 的高度/大小（顺序不能反）
 */
static void rotate_right(Node** root_ptr, Node* NIL, Node* pivot) {
    Node* new_root = pivot->left;
    Node* parent = pivot->parent;
    Node* b = pivot->left->right;

    pivot->left = b;
    if (b != NIL) b->parent = pivot;

    new_root->right = pivot;
    pivot->parent = new_root;

    new_root->parent = parent;
    if (parent == NIL) *root_ptr = new_root;
    else if (pivot == parent->left) parent->left = new_root;
    else parent->right = new_root;

    update_height(NIL, pivot);
    update_height(NIL, new_root);
}

/**
 * 左旋（RR 情形）
 *
 *   pivot                     new_root
 *      \                    /         \
 *     new_root      →    pivot        b
 *        / \                \
 *       ?   b                ?
 *
 * 与右旋对称，方向相反。
 */
static void rotate_left(Node** root_ptr, Node* NIL, Node* pivot) {
    Node* new_root = pivot->right;
    Node* parent = pivot->parent;
    Node* b = pivot->right->left;

    pivot->right = b;
    if (b != NIL) b->parent = pivot;

    new_root->left = pivot;
    pivot->parent = new_root;

    new_root->parent = parent;
    if (parent == NIL) *root_ptr = new_root;
    else if (pivot == parent->left) parent->left = new_root;
    else parent->right = new_root;

    update_height(NIL, pivot);
    update_height(NIL, new_root);
}

/** Compute balance factor = left->height - right->height */
static int get_balance(Node* NIL, Node* node) {
    if (node == NIL) return 0;
    return get_height(NIL, node->left) - get_height(NIL, node->right);
}

/**
 * Check pivot's balance factor and perform rotations if needed.
 * Called upward from the insertion/deletion point along the path.
 */
static void rebalance(Node** root_ptr, Node* NIL, Node* pivot) {
    int balance = get_balance(NIL, pivot);
    if (abs(balance) <= 1) return;
    if (balance > 0) { // balance > 1, L heavy
        if (get_balance(NIL, pivot->left) < 0) // LR
            rotate_left(root_ptr, NIL, pivot->left);
        rotate_right(root_ptr, NIL, pivot);
    } else { // balance < -1, R heavy
        if (get_balance(NIL, pivot->right) > 0) // RL
            rotate_right(root_ptr, NIL, pivot->right);
        rotate_left(root_ptr, NIL, pivot);
    }
}

/**
 * Recursively insert a node.
 * Insert at leaf position per BST rules, then update height/size
 * and check balance during backtrack, rotating if unbalanced.
 */
static void insert(Node** root_ptr, Node* NIL, Node* root, Node* node) {
    if (node->val < root->val) {
        if (root->left == NIL) {
            node->parent = root;
            root->left = node;
        } else {
            insert(root_ptr, NIL, root->left, node);
        }
    } else {
        if (root->right == NIL) {
            node->parent = root;
            root->right = node;
        } else {
            insert(root_ptr, NIL, root->right, node);
        }
    }
    update_height(NIL, root);
    rebalance(root_ptr, NIL, root);
}

/** Find the minimum node in the subtree rooted at node (leftmost) */
static Node* find_min(Node* NIL, Node* node) {
    if (node == NIL) return NIL;
    while (node->left != NIL) node = node->left;
    return node;
}

/**
 * Recursively delete a value.
 *
 * Three cases:
 *   1. Leaf or has at most one child -> remove directly, replace with child
 *   2. Has both children -> overwrite with successor (min of right subtree),
 *      then recursively delete the successor (reduces to case 1)
 * After deletion, update height/size and rebalance along the backtrack path.
 */
static void delete_val(Node** root_ptr, Node* NIL, Node* root, int val) {
    if (root == NIL) return;
    if (val > root->val) delete_val(root_ptr, NIL, root->right, val);
    else if (val < root->val) delete_val(root_ptr, NIL, root->left, val);
    else { // root->val == val
        if (root->left == NIL) {
            Node* child = root->right;
            if (root == *root_ptr) *root_ptr = child; 
            else if (root == root->parent->left) root->parent->left = root->right;
            else root->parent->right = root->right;

            if (child != NIL) child->parent = root->parent;
            return;
        } else if (root->right == NIL) {
            Node* child = root->left;
            if (root == *root_ptr) *root_ptr = child;
            else if (root == root->parent->left) root->parent->left = root->left;
            else root->parent->right = root->left;
            
            if (child != NIL) child->parent = root->parent;
            return;
        } else {
            Node* successor = find_min(NIL, root->right);
            root->val = successor->val;
            delete_val(root_ptr, NIL, root->right, successor->val);
        }
    }
    update_height(NIL, root);
    rebalance(root_ptr, NIL, root);
}

/** Find a node with the given value in the BST */
static Node* find(Node* root, Node* NIL, int val) {
    while (root != NIL) {
        if (val < root->val) root = root->left;
        else if (val > root->val) root = root->right;
        else return root;
    }
    return NIL;
}

/** Find the first node > val (strict successor) */
static Node* upper_bound(Node* root, Node* NIL, int val) {
    Node* result = NIL;
    while (root != NIL) {
        if (root->val > val) {
            result = root;        // 当前节点是一个候选答案
            root = root->left;    // 尝试找更小的满足条件的节点
        } else {
            root = root->right;   // 当前节点太小，去右子树找
        }
    }
    return result;
}

/** Find the first node >= val */
static Node* lower_bound(Node* root, Node* NIL, int val) {
    Node* result = NIL;
    while (root != NIL) {
        if (root->val >= val) {
            result = root;
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return result;
}

/**
 * Find the node with rank k (1-indexed).
 * Uses subtree size to decide whether to go left or right.
 */
static Node* kth(Node* NIL, Node* root, int k) {  // 1-indexed
    if (root == NIL) return NIL;
    int left_size = (root->left == NIL) ? 0 : root->left->size;
    if (k <= left_size) return kth(NIL, root->left, k);
    if (k == left_size + 1) return root;
    return kth(NIL, root->right, k - left_size - 1);
}

/**
 * Compute the rank of val (1-indexed).
 * Accumulates left subtree sizes:
 *   If val <= current value, rank is in the left subtree;
 *   Otherwise accumulate left_size + 1 (current node) and go right.
 */
static int find_rank(Node* NIL, Node* root, int val) {
    int rank = 0;
    while (root != NIL) {
        if (val <= root->val) {
            // 当前节点 >= val，所以首个 val 必然在左子树，直接向左走
            root = root->left;
        } else {
            // val > root->val，当前节点和左子树全部严格小于 val
            rank += root->left->size + 1;
            root = root->right;
        }
    }
    return rank + 1;
}

AVL_Tree* avl_tree_create(int n) {
    if (n <= 0) return NULL;

    AVL_Tree* tree = (AVL_Tree*)malloc(sizeof(AVL_Tree));
    if (!tree) return NULL;

    tree->nodes = (Node*)malloc((n + 1) * sizeof(Node));
    if (!tree->nodes) {
        free(tree);
        return NULL;
    }

    tree->idx = 0;
    tree->capacity = n;
    tree->nil = &tree->nodes[0];
    tree->nil->parent = tree->nil;
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil;
    tree->nil->height = 0;
    tree->nil->val = 0;
    tree->nil->size = 0;

    tree->root = tree->nil;

    return tree;
}

void avl_tree_insert(AVL_Tree* tree, int val) {
    if (tree->idx >= tree->capacity) return;

    int idx = ++tree->idx;
    Node* node = &tree->nodes[idx];
    node->parent = tree->nil;
    node->left = tree->nil;
    node->right = tree->nil;
    node->height = 1;
    node->size = 1;
    node->val = val;

    if (tree->root == tree->nil) {
        tree->root = node;
        return;
    }

    insert(&tree->root, tree->nil, tree->root, node);
}

void avl_tree_erase(AVL_Tree* tree, int val) {
    if (tree->root == tree->nil) return;
    delete_val(&tree->root, tree->nil, tree->root, val);
}

Node* avl_tree_find(AVL_Tree* tree, int val) {
    return find(tree->root, tree->nil, val);
}

int avl_tree_find_val(AVL_Tree* tree, int val) {
    return find(tree->root, tree->nil, val)->val;
}

void avl_tree_destroy(AVL_Tree* tree) {
    if (!tree) return;
    if (tree->nodes) free(tree->nodes);
    free(tree);
}

Node* avl_tree_upper_bound(AVL_Tree* tree, int val) {
    return upper_bound(tree->root, tree->nil, val);     
}

Node* avl_tree_lower_bound(AVL_Tree* tree, int val) {
    return lower_bound(tree->root, tree->nil, val);
}

Node* avl_tree_next(AVL_Tree* tree, int val) {
    return upper_bound(tree->root, tree->nil, val);
}

int avl_tree_next_val(AVL_Tree* tree, int val) {
    return avl_tree_next(tree, val)->val;
}

Node* avl_tree_prev(AVL_Tree* tree, int val) {
    Node* cur = tree->root;
    Node* pred = tree->nil;
    while (cur != tree->nil) {
        if (cur->val < val) {
            pred = cur;              // 当前节点是候选前驱
            cur = cur->right;        // 找更大的
        } else {
            cur = cur->left;         // 当前节点 >= val，去左子树找更小的候选
        }
    }
    return pred;
}

int avl_tree_prev_val(AVL_Tree* tree, int val) {
    return avl_tree_prev(tree, val)->val;
}

Node* avl_tree_kth(AVL_Tree* tree, int k) {
    return kth(tree->nil, tree->root, k);
}

int avl_tree_kth_val(AVL_Tree* tree, int k) {
    return avl_tree_kth(tree, k)->val;
}

int avl_tree_rank(AVL_Tree* tree, int val) {
    return find_rank(tree->nil, tree->root, val);
}