// This is the header of rb tree.
#ifndef RB_TREE_H
#define RB_TREE_H

typedef struct Node Node;

typedef struct {
    Node* root;
    Node* nil;
    Node* nodes;
    int idx;
    int capacity;
} RB_Tree;

RB_Tree* rb_tree_create(int n);
void rb_tree_insert(RB_Tree* tree, int val);
void rb_tree_erase(RB_Tree* tree, int val);
Node* rb_tree_find(RB_Tree* tree, int val);
int rb_tree_find_val(RB_Tree* tree, int val);
void rb_tree_destroy(RB_Tree* tree);
Node* rb_tree_upper_bound(RB_Tree* tree, int val);
Node* rb_tree_lower_bound(RB_Tree* tree, int val);
Node* rb_tree_next(RB_Tree* tree, int val);
int rb_tree_next_val(RB_Tree* tree, int val);
Node* rb_tree_prev(RB_Tree* tree, int val);
int rb_tree_prev_val(RB_Tree* tree, int val);
Node* rb_tree_kth(RB_Tree* tree, int k);
int rb_tree_kth_val(RB_Tree* tree, int k);
int rb_tree_rank(RB_Tree* tree, int val);

#endif