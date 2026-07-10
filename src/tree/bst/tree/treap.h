// This is the header of treap.
#ifndef TREAP_H
#define TREAP_H

#include <utensils/rng.h>

typedef struct Node Node;

typedef struct {
    Node* root;
    Node* nil;
    Node* nodes;
    int idx;
    int capacity;
    RNG* rng;
} Treap;

Treap* treap_create(int n);
void treap_insert(Treap* tree, int val);
void treap_erase(Treap* tree, int val);
Node* treap_find(Treap* tree, int val);
int treap_find_val(Treap* tree, int val);
void treap_destroy(Treap* tree);
Node* treap_upper_bound(Treap* tree, int val);
Node* treap_lower_bound(Treap* tree, int val);
Node* treap_next(Treap* tree, int val);
int treap_next_val(Treap* tree, int val);
Node* treap_prev(Treap* tree, int val);
int treap_prev_val(Treap* tree, int val);
Node* treap_kth(Treap* tree, int k);
int treap_kth_val(Treap* tree, int k);
int treap_rank(Treap* tree, int val);

#endif