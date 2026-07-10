/**
 * @file splay.c
 * @brief Splay tree implementation
 *
 * Core idea: splay the accessed node to the root on every operation,
 * leveraging locality of reference to keep frequently accessed nodes near the top.
 */
#include <stdlib.h>
#include <stdio.h>
#include "splay.h"

/**
 * Splay tree node
 * @param parent Parent node
 * @param left   Left child
 * @param right  Right child
 * @param val    Node value
 * @param size   Subtree size
 * @param cnt    Duplicate count
 */
typedef struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int val;
    int size;
    int cnt;
} Node;

/** Update node size information */
static void update_size(Node* node, Node* NIL) {
    if (node == NIL) return;
    node->size = node->cnt + node->left->size + node->right->size;
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
 * Splay a node to the root via rotations
 *
 * Three patterns:
 *   zig:      single rotation when parent is the root
 *   zig-zig:  node and parent on the same side -> rotate parent first, then node
 *   zig-zag:  node and parent on opposite sides -> rotate node first, then the other side
 */
static void splay(Node** root_ptr, Node* node, Node* NIL) {
    if (node == NIL || node == *root_ptr) return;
    Node* parent = node->parent;
    Node* grandparent = parent->parent;
    while (node->parent != NIL) {
        if (grandparent == NIL) { // zig
            if (node == parent->left) rotate_right(root_ptr, parent, NIL);
            else rotate_left(root_ptr, parent, NIL);
            break;
        }
        if (parent == grandparent->left && node == parent->left) { // L-/
            rotate_right(root_ptr, grandparent, NIL);
            rotate_right(root_ptr, parent, NIL);
        } else if (parent == grandparent->left && node == parent->right) { // <
            rotate_left(root_ptr, parent, NIL);
            rotate_right(root_ptr, grandparent, NIL);
        } else if (parent == grandparent->right && node == parent->left) { // >
            rotate_right(root_ptr, parent, NIL);
            rotate_left(root_ptr, grandparent, NIL);
        } else { // R-/
            rotate_left(root_ptr, grandparent, NIL);
            rotate_left(root_ptr, parent, NIL);
        }
        parent = node->parent;
        grandparent = parent->parent;
    }
}

/** Insert a node, then splay it to the root */
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
        else if (node->val > cur->val) cur = cur->right;
        else {
            cur->cnt++;
            splay(root_ptr, cur, NIL);
            return;
        }
    }

    node->parent = parent;
    if (node->val < parent->val) parent->left = node;
    else parent->right = node;

    splay(root_ptr, node, NIL);
}

/**
 * Delete a node by value.
 * Splay the target to the root, then merge left and right subtrees.
 */
static void erase(Node** root_ptr, int val, Node* NIL) {
    Node* cur = *root_ptr;
    Node* parent = NIL;
    while (cur != NIL) {
        parent = cur;
        if (val < cur->val) cur = cur->left;
        else if (val > cur->val) cur = cur->right;
        else break;
    }

    if (cur == NIL) {
        splay(root_ptr, parent, NIL);
        return;
    }
    
    if (cur->cnt > 1) {
        cur->cnt--;
        splay(root_ptr, cur, NIL);
        return;
    }

    splay(root_ptr, cur, NIL);

    if (cur->right == NIL) {
        *root_ptr = cur->left;
        cur->left->parent = NIL;
        return;
    }

    Node* successor = cur->right;
    while (successor->left != NIL) successor = successor->left;

    splay(root_ptr, successor, NIL);

    successor->left = cur->left;
    if (cur->left != NIL) cur->left->parent = successor;

    update_size(successor, NIL);
}

/** Find a node by value (no splay) */
static Node* find(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    while (node != NIL) {
        if (val < node->val) node = node->left;
        else if (val > node->val) node = node->right;
        else break;
    }
    if (node != NIL) {
        // printf("find val %d\n", val);
        splay(root_ptr, node, NIL);
        return node;
    }
    return NIL;
}

/** Compute rank of val (1-indexed) */
static int rank(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* parent = NIL;
    int rank = 0;
    while (node != NIL) {
        parent = node;
        if (val <= node->val) node = node->left;
        else rank += node->left->size + node->cnt, node = node->right;
    }
    splay(root_ptr, parent, NIL);
    return rank + 1;
} 

/** Get node with rank k (1-indexed) */
static Node* kth(Node** root_ptr, int k, Node* NIL) {
    Node* node = *root_ptr;
    while (node != NIL) {
        if (node->left->size >= k) node = node->left;
        else if (node->left->size + node->cnt >= k) {
            splay(root_ptr, node, NIL);
            return node;
        }
        else k -= node->left->size + node->cnt, node = node->right;
    }
    return NIL;
}

/** Find predecessor (largest node < val) */
static Node* prev(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* prev = NIL;
    Node* last = NIL;
    while (node != NIL) {
        last = node;
        if (node->val < val) prev = node, node = node->right;
        else node = node->left;
    }
    if (prev == NIL) splay(root_ptr, last, NIL);
    if (prev != NIL) splay(root_ptr, prev, NIL);
    return prev;
}

/** Find first node >= val */
static Node* lower_bound(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* next = NIL;
    Node* last = NIL;
    while (node != NIL) {
        last = node;
        if (node->val >= val) next = node, node = node->left;
        else node = node->right;
    }
    if (next == NIL) splay(root_ptr, last, NIL);
    if (next != NIL) splay(root_ptr, next, NIL);
    return next;
}

/** Find first node > val */
static Node* upper_bound(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* next = NIL;
    Node* last = NIL;
    while (node != NIL) {
        last = node;
        if (node->val > val) next = node, node = node->left;
        else node = node->right;
    }
    if (next == NIL) splay(root_ptr, last, NIL);
    if (next != NIL) splay(root_ptr, next, NIL);
    return next;
}

/**
 * Create a splay tree
 * @param n Maximum capacity
 * @return Pointer to the tree on success, NULL on failure
 */
Splay* splay_create(int n) {
    if (n <= 0) return NULL;

    Splay* tree = (Splay*)malloc(sizeof(Splay));
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
    tree->nil->cnt = 0;

    tree->root = tree->nil;
    tree->idx = 0;
    tree->capacity = n;
    return tree;
}

/** Destroy the tree */
void splay_destroy(Splay* tree) {
    if (!tree) return;
    free(tree->nodes);
    free(tree);
}

/** Insert a value */
void splay_insert(Splay* tree, int val) {
    if (tree->idx >= tree->capacity) return;

    Node* node = &tree->nodes[++tree->idx];
    node->val = val;
    node->parent = tree->nil;
    node->left = tree->nil;
    node->right = tree->nil;
    node->size = 1;
    node->cnt = 1;

    insert(&tree->root, node, tree->nil);
}

/** Erase a value */
void splay_erase(Splay* tree, int val) {
    erase(&tree->root, val, tree->nil);
}

/** Find a node by value */
Node* splay_find(Splay* tree, int val) {
    return find(&tree->root, val, tree->nil);
}

/** Find a value and return it */
int splay_find_val(Splay* tree, int val) {
    return find(&tree->root, val, tree->nil)->val;
}

/** Get rank of val (1-indexed) */
int splay_rank(Splay* tree, int val) {
    return rank(&tree->root, val, tree->nil);
}

/** Get node with rank k (1-indexed) */
Node* splay_kth(Splay* tree, int k) {
    return kth(&tree->root, k, tree->nil);
}

/** Get value with rank k (1-indexed) */
int splay_kth_val(Splay* tree, int k) {
    return kth(&tree->root, k, tree->nil)->val;
}

/** Get predecessor of val */
Node* splay_prev(Splay* tree, int val) {
    return prev(&tree->root, val, tree->nil);
}

/** Get predecessor value of val */
int splay_prev_val(Splay* tree, int val) {
    return prev(&tree->root, val, tree->nil)->val;
}

/** Get first node >= val */
Node* splay_lower_bound(Splay* tree, int val) {
    return lower_bound(&tree->root, val, tree->nil);
}

/** Get first node > val */
Node* splay_upper_bound(Splay* tree, int val) {
    return upper_bound(&tree->root, val, tree->nil);
}

/** Get successor of val */
Node* splay_next(Splay* tree, int val) {
    return upper_bound(&tree->root, val, tree->nil);
}

/** Get successor value of val */
int splay_next_val(Splay* tree, int val) {
    return upper_bound(&tree->root, val, tree->nil)->val;
}

