// This is the header of the AVL Tree. 
#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct Node Node;

typedef struct {
    Node* root;
    Node* nil;
    Node* nodes;
    int idx;
    int capacity;
} AVL_Tree;

AVL_Tree* avl_tree_create(int n);
void avl_tree_insert(AVL_Tree* tree, int val);
void avl_tree_delete(AVL_Tree* tree, int val);
Node* avl_tree_find(AVL_Tree* tree, int val);
int avl_tree_find_val(AVL_Tree* tree, int val);
void avl_tree_destroy(AVL_Tree* tree);
Node* avl_tree_upper_bound(AVL_Tree* tree, int val);
Node* avl_tree_lower_bound(AVL_Tree* tree, int val);
Node* avl_tree_next(AVL_Tree* tree, int val);
int avl_next_val(AVL_Tree* tree, int val);
Node* avl_tree_prev(AVL_Tree* tree, int val);
int avl_tree_prev_val(AVL_Tree* tree, int val);
Node* avl_tree_kth(AVL_Tree* tree, int k);
int avl_tree_kth_val(AVL_Tree* tree, int k);
int avl_tree_rank(AVL_Tree* tree, int val);

#endif