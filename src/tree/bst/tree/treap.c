#include <stdlib.h>
#include <stdint.h>

#include <utensils/rng.h>
#include "treap.h"

typedef struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int val;
    int size;
    uint32_t priority;
} Node;

static void update_size(Node* node, Node* NIL) {
    if (node == NIL) return;
    node->size = 1 + node->left->size + node->right->size;
}

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

static Node* find(Node* node, int val, Node* NIL) {
    while (node != NIL) {
        if (val < node->val) node = node->left;
        else if (val > node->val) node = node->right;
        else return node;
    }
    return NIL;
}

static int rank(Node* node, int val, Node* NIL) {
    int rank = 0;
    while (node != NIL) {
        if (val <= node->val) node = node->left;
        else rank += node->left->size + 1, node = node->right;
    }
    return rank + 1;
} 

static Node* kth(Node* node, int k, Node* NIL) {
    while (node != NIL) {
        if (node->left->size >= k) node = node->left;
        else if (node->left->size == k - 1) return node;
        else k -= node->left->size + 1, node = node->right;
    }
    return NIL;
}

static Node* prev(Node* node, int val, Node* NIL) {
    Node* prev = NIL;
    while (node != NIL) {
        if (node->val < val) prev = node, node = node->right;
        else node = node->left;
    }
    return prev;
}

static Node* lower_bound(Node* node, int val, Node* NIL) {
    Node* next = NIL;
    while (node != NIL) {
        if (node->val >= val) next = node, node = node->left;
        else node = node->left;
    }
    return next;
}

static Node* upper_bound(Node* node, int val, Node* NIL) {
    Node* next = NIL;
    while (node != NIL) {
        if (node->val > val) next = node, node = node->left;
        else node = node->right;
    }
    return next;
}

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

void treap_destroy(Treap* tree) {
    if (!tree) return;
    free(tree->nodes);
    free(tree);
}

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

void treap_erase(Treap* tree, int val) {
    erase(&tree->root, val, tree->nil);
}

Node* treap_find(Treap* tree, int val) {
    return find(tree->root, val, tree->nil);
}

int treap_find_val(Treap* tree, int val) {
    return find(tree->root, val, tree->nil)->val;
}

int treap_rank(Treap* tree, int val) {
    return rank(tree->root, val, tree->nil);
}

Node* treap_kth(Treap* tree, int k) {
    return kth(tree->root, k, tree->nil);
}

int treap_kth_val(Treap* tree, int k) {
    return kth(tree->root, k, tree->nil)->val;
}

Node* treap_prev(Treap* tree, int val) {
    return prev(tree->root, val, tree->nil);
}

int treap_prev_val(Treap* tree, int val) {
    return prev(tree->root, val, tree->nil)->val;
}

Node* treap_lower_bound(Treap* tree, int val) {
    return lower_bound(tree->root, val, tree->nil);
}

Node* treap_upper_bound(Treap* tree, int val) {
    return upper_bound(tree->root, val, tree->nil);
}

Node* treap_next(Treap* tree, int val) {
    return upper_bound(tree->root, val, tree->nil);
}

int treap_next_val(Treap* tree, int val) {
    return upper_bound(tree->root, val, tree->nil)->val;
}

