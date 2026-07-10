#include <stdio.h>
#include <stdlib.h>
#include "rb_tree.h"

typedef enum Color {BLACK = 0, RED = 1} Color;

typedef struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int val;
    int size;
    Color color;  
} Node;

/** Update subtree size */
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

/** Fix red-black properties after insertion */
static void insert_fixup(Node** root_ptr, Node* node, Node* NIL) {
    while (node->parent->color == RED) {
        Node* parent = node->parent;
        Node* grandparent = parent->parent;
        Node* uncle;
        if (parent == grandparent->left) uncle = grandparent->right;
        else uncle = grandparent->left;

        if (uncle->color == RED) { // Case 3: P & U are red: bubbling
            parent->color = BLACK;
            uncle->color = BLACK;
            if (grandparent != NIL) grandparent->color = RED;
            node = grandparent;
            continue;
        } 
        
        // Case 4: parent is red and uncle is black: we need rotation.

        if (parent == grandparent->left) { // L
            if (node == parent->right) rotate_left(root_ptr, parent, NIL), parent = node; // LR
            parent->color = BLACK;
            if (grandparent!= NIL) grandparent->color = RED;
            rotate_right(root_ptr, grandparent, NIL);
            break;
        } else { // R
            if (node == parent->left) rotate_right(root_ptr, parent, NIL), parent = node; // RL
            parent->color = BLACK;
            if (grandparent != NIL) grandparent->color = RED;
            rotate_left(root_ptr, grandparent, NIL);
            break;
        }
    }
    (*root_ptr)->color = BLACK;  // Case 1 & 2: make root's color black, then return.
}

/** Insert a node into the red-black tree */
static void insert(Node** root_ptr, Node* node, Node* NIL) {
    if (*root_ptr == NIL) { // case 1: new node is the root.
        *root_ptr = node;
        node->color = RED;
        return;
    }
    Node* cur = *root_ptr;
    Node* parent = NIL;
    while (cur != NIL) { // iteration to find parent node.
        parent = cur;
        parent->size++;
        if (node->val < cur->val) cur = cur->left;
        else cur = cur->right;
    }
    node->parent = parent;
    if (node->val < parent->val) parent->left = node;
    else parent->right = node;

    insert_fixup(root_ptr, node, NIL);
}

/** Fix red-black properties after deletion */
static void erase_fixup(Node** root_ptr, Node* node, Node* NIL) {
    while (node != (*root_ptr) && node->color != RED) {
        Node* parent = node->parent;
        Node* sibling = (node == parent->left) ? parent->right : parent->left;

        if (sibling->color == RED) {
            sibling->color = BLACK;
            parent->color = RED;
            if (node == parent->left) {
                rotate_left(root_ptr, parent, NIL);
                sibling = parent->right;
            } else {
                rotate_right(root_ptr, parent, NIL);
                sibling = parent->left;
            }
        }

        if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
            sibling->color = RED;
            if (parent->color == RED) {
                parent->color = BLACK;
                break;
            }
            node = parent;
            continue;
        }

        if ((node == parent->left && sibling->right->color == BLACK)
        || (node == parent->right && sibling->left->color == BLACK)) {
            sibling->color = RED;
            if (node == parent->left) {
                sibling->left->color = BLACK;
                rotate_right(root_ptr, sibling, NIL);
                sibling = parent->right;
            } else {
                sibling->right->color = BLACK;
                rotate_left(root_ptr, sibling, NIL);
                sibling = parent->left;
            }
        } 

        sibling->color = parent->color;
        parent->color = BLACK;
        if (node == parent->left) {
            sibling->right->color = BLACK;
            rotate_left(root_ptr, parent, NIL);
        } else {
            sibling->left->color = BLACK;
            rotate_right(root_ptr, parent, NIL);
        }
        break;
    }
    node->color = BLACK;
}

/** Delete a node by value from the red-black tree */
static void erase(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* target = NIL;
    Node* child = NIL;

    while (node != NIL) {
        if (val < node->val) node = node->left;
        else if (val > node->val) node = node->right;
        else {
            if (node->left == NIL || node->right == NIL) { // situation 1: one child or no child
                if (node->left == NIL) child = node->right;
                else child = node->left;

                if (child != NIL) child->parent = node->parent;

                if (node->parent == NIL) *root_ptr = child;
                else if (node == node->parent->left) node->parent->left = child; 
                else node->parent->right = child;
                
                target = node;     
            } else { // situation 2: two childs
                Node* successor = node->right;
                while (successor->left != NIL) successor = successor->left;
                node->val = successor->val;
                
                // then go to the situation 1.
                if (successor->right != NIL) child = successor->right;
                
                if (child != NIL) child->parent = successor->parent;
                
                if (successor == successor->parent->left) successor->parent->left = child; 
                else successor->parent->right = child;
                target = successor;
            }
            break;
        }
    }

    if (target == NIL) return;

    Node* cur = target->parent;
    while (cur != NIL) update_size(cur, NIL), cur = cur->parent;

    if (target->color == RED) return;
    
    if (child == NIL) NIL->parent = target->parent;
    erase_fixup(root_ptr, child, NIL);
    NIL->parent = NIL;
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

/** Get the node with rank k (1-indexed) */
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

RB_Tree* rb_tree_create(int n) {
    if (n <= 0) return NULL;

    RB_Tree* tree = (RB_Tree*)malloc(sizeof(RB_Tree));
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
    tree->nil->color = BLACK;

    tree->root = tree->nil;
    tree->idx = 0;
    tree->capacity = n;
    return tree;
}

void rb_tree_destroy(RB_Tree* tree) {
    if (!tree) return;
    free(tree->nodes);
    free(tree);
}

void rb_tree_insert(RB_Tree* tree, int val) {
    if (tree->idx >= tree->capacity) return;

    Node* node = &tree->nodes[++tree->idx];
    node->val = val;
    node->parent = tree->nil;
    node->left = tree->nil;
    node->right = tree->nil;
    node->size = 1;
    node->color = RED;

    insert(&tree->root, node, tree->nil);
}

void rb_tree_erase(RB_Tree* tree, int val) {
    erase(&tree->root, val, tree->nil);
}

Node* rb_tree_find(RB_Tree* tree, int val) {
    return find(tree->root, val, tree->nil);
}

int rb_tree_find_val(RB_Tree* tree, int val) {
    return find(tree->root, val, tree->nil)->val;
}

int rb_tree_rank(RB_Tree* tree, int val) {
    return rank(tree->root, val, tree->nil);
}

Node* rb_tree_kth(RB_Tree* tree, int k) {
    return kth(tree->root, k, tree->nil);
}

int rb_tree_kth_val(RB_Tree* tree, int k) {
    return kth(tree->root, k, tree->nil)->val;
}

Node* rb_tree_prev(RB_Tree* tree, int val) {
    return prev(tree->root, val, tree->nil);
}

int rb_tree_prev_val(RB_Tree* tree, int val) {
    return prev(tree->root, val, tree->nil)->val;
}

Node* rb_tree_lower_bound(RB_Tree* tree, int val) {
    return lower_bound(tree->root, val, tree->nil);
}

Node* rb_tree_upper_bound(RB_Tree* tree, int val) {
    return upper_bound(tree->root, val, tree->nil);
}

Node* rb_tree_next(RB_Tree* tree, int val) {
    return upper_bound(tree->root, val, tree->nil);
}

int rb_tree_next_val(RB_Tree* tree, int val) {
    return upper_bound(tree->root, val, tree->nil)->val;
}
