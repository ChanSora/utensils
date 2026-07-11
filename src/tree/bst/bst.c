/**
 * @file bst.c
 * @brief Polymorphic BST dispatch layer
 *
 * This file is the single compilation unit that maps the abstract Tree
 * vtable to the four concrete tree backends (AVL, RB, Treap, Splay).
 *
 * Each vtable entry is a thin wrapper (e.g. bst_insert) that switches on
 * tree->type and delegates to the corresponding backend function after
 * casting the opaque `data` pointer to the correct concrete type.
 *
 * Locking / error handling are intentionally omitted to keep the
 * dispatch as cheap as possible — the caller is expected to guard against
 * misuse (e.g. calling operations before create()).
 */

#include <utensils/tree.h>

#include <stdlib.h>

#include "tree/rb_tree.h"
#include "tree/avl_tree.h"
#include "tree/treap.h"
#include "tree/splay.h"

/* ------------------------------------------------------------------ */
/* Forward declarations — each maps to one vtable slot                 */
/* ------------------------------------------------------------------ */

static void bst_create(Tree* tree, int capacity);
/** Deallocate the concrete tree backend */
static void bst_destroy(Tree* tree);
/** Insert a value into the tree */
static void bst_insert(Tree* tree, int val);
/** Erase a value from the tree */
static void bst_erase(Tree* tree, int val);
/** Find a node by value (returns NULL if not found) */
static void* bst_find(Tree* tree, int val);
/** Find a value and return it */
static int bst_find_val(Tree* tree, int val);
/** Get the rank of val (1-indexed) */
static int bst_rank(Tree* tree, int val);
/** Get the node with rank k (1-indexed) */
static void* bst_kth(Tree* tree, int k);
/** Get the value with rank k (1-indexed) */
static int bst_kth_val(Tree* tree, int k);
/** Get the first node >= val */
static void* bst_lower_bound(Tree* tree, int val);
/** Get the first node > val */
static void* bst_upper_bound(Tree* tree, int val);
/** Get the predecessor of val */
static void* bst_prev(Tree* tree, int val);
/** Get the predecessor value of val */
static int bst_prev_val(Tree* tree, int val);
/** Get the successor of val */
static void* bst_next(Tree* tree, int val);
/** Get the successor value of val */
static int bst_next_val(Tree* tree, int val);

/* ================================================================== */
/*  tree_init — vtable initialisation                                  */
/* ================================================================== */

Tree tree_init(TreeType type) {
    Tree tree;
    tree.type = type;
    tree.data = NULL;
    tree.create = bst_create;
    tree.destroy = bst_destroy;
    tree.insert = bst_insert;
    tree.erase = bst_erase;
    tree.find = bst_find;
    tree.find_val = bst_find_val;
    tree.rank = bst_rank;
    tree.kth = bst_kth;
    tree.kth_val = bst_kth_val;
    tree.lower_bound = bst_lower_bound;
    tree.upper_bound = bst_upper_bound;
    tree.prev = bst_prev;
    tree.prev_val = bst_prev_val;
    tree.next = bst_next;
    tree.next_val = bst_next_val;
    return tree;
}

/* ================================================================== */
/*  Dispatch wrappers — type-erased switch on tree->type              */
/* ================================================================== */

/** Allocate the concrete tree backend */
static void bst_create(Tree* tree, int capacity) {
    switch (tree->type) {
        case RB_TREE:   tree->data = rb_tree_create(capacity); break;
        case AVL_TREE:  tree->data = avl_tree_create(capacity); break;
        case TREAP:     tree->data = treap_create(capacity); break;
        case SPLAY:     tree->data = splay_create(capacity); break;
        default:        tree->data = NULL; break;
    }
}

/** Deallocate the concrete tree backend */
static void bst_destroy(Tree* tree) {
    if (!tree->data) return;
    switch (tree->type) {
        case RB_TREE:   rb_tree_destroy((RB_Tree*)tree->data); break;
        case AVL_TREE:  avl_tree_destroy((AVL_Tree*)tree->data); break;
        case TREAP:     treap_destroy((Treap*)tree->data); break;
        case SPLAY:     splay_destroy((Splay*)tree->data); break;
        default: break;
    }
    tree->data = NULL;
}

/** Insert a value into the tree */
static void bst_insert(Tree* tree, int val) {
    if (!tree->data) return;
    switch (tree->type) {
        case RB_TREE:   rb_tree_insert((RB_Tree*)tree->data, val); break;
        case AVL_TREE:  avl_tree_insert((AVL_Tree*)tree->data, val); break;
        case TREAP:     treap_insert((Treap*)tree->data, val); break;
        case SPLAY:     splay_insert((Splay*)tree->data, val); break;
        default: break;
    }
}

/** Erase a value from the tree */
static void bst_erase(Tree* tree, int val) {
    if (!tree->data) return;
    switch (tree->type) {
        case RB_TREE:   rb_tree_erase((RB_Tree*)tree->data, val); break;
        case AVL_TREE:  avl_tree_erase((AVL_Tree*)tree->data, val); break;
        case TREAP:     treap_erase((Treap*)tree->data, val); break;
        case SPLAY:     splay_erase((Splay*)tree->data, val); break;
        default: break;
    }
}

/** Find a node by value (returns NULL if not found) */
static void* bst_find(Tree* tree, int val) {
    if (!tree->data) return NULL;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_find((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_find((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_find((Treap*)tree->data, val);
        case SPLAY:     return splay_find((Splay*)tree->data, val);
        default:        return NULL;
    }
}

/** Find a value and return it */
static int bst_find_val(Tree* tree, int val) {
    if (!tree->data) return 0;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_find_val((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_find_val((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_find_val((Treap*)tree->data, val);
        case SPLAY:     return splay_find_val((Splay*)tree->data, val);
        default:        return 0;
    }
}

/** Get the rank of val (1-indexed) */
static int bst_rank(Tree* tree, int val) {
    if (!tree->data) return 0;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_rank((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_rank((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_rank((Treap*)tree->data, val);
        case SPLAY:     return splay_rank((Splay*)tree->data, val);
        default:        return 0;
    }
}

/** Get the node with rank k (1-indexed) */
static void* bst_kth(Tree* tree, int k) {
    if (!tree->data) return NULL;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_kth((RB_Tree*)tree->data, k);
        case AVL_TREE:  return avl_tree_kth((AVL_Tree*)tree->data, k);
        case TREAP:     return treap_kth((Treap*)tree->data, k);
        case SPLAY:     return splay_kth((Splay*)tree->data, k);
        default:        return NULL;
    }
}

/** Get the value with rank k (1-indexed) */
static int bst_kth_val(Tree* tree, int k) {
    if (!tree->data) return 0;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_kth_val((RB_Tree*)tree->data, k);
        case AVL_TREE:  return avl_tree_kth_val((AVL_Tree*)tree->data, k);
        case TREAP:     return treap_kth_val((Treap*)tree->data, k);
        case SPLAY:     return splay_kth_val((Splay*)tree->data, k);
        default:        return 0;
    }
}

/** Get the first node >= val */
static void* bst_lower_bound(Tree* tree, int val) {
    if (!tree->data) return NULL;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_lower_bound((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_lower_bound((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_lower_bound((Treap*)tree->data, val);
        case SPLAY:     return splay_lower_bound((Splay*)tree->data, val);
        default:        return NULL;
    }
}

/** Get the first node > val */
static void* bst_upper_bound(Tree* tree, int val) {
    if (!tree->data) return NULL;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_upper_bound((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_upper_bound((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_upper_bound((Treap*)tree->data, val);
        case SPLAY:     return splay_upper_bound((Splay*)tree->data, val);
        default:        return NULL;
    }
}

/** Get the predecessor of val */
static void* bst_prev(Tree* tree, int val) {
    if (!tree->data) return NULL;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_prev((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_prev((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_prev((Treap*)tree->data, val);
        case SPLAY:     return splay_prev((Splay*)tree->data, val);
        default:        return NULL;
    }
}

/** Get the predecessor value of val */
static int bst_prev_val(Tree* tree, int val) {
    if (!tree->data) return 0;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_prev_val((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_prev_val((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_prev_val((Treap*)tree->data, val);
        case SPLAY:     return splay_prev_val((Splay*)tree->data, val);
        default:        return 0;
    }
}

/** Get the successor of val */
static void* bst_next(Tree* tree, int val) {
    if (!tree->data) return NULL;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_next((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_next((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_next((Treap*)tree->data, val);
        case SPLAY:     return splay_next((Splay*)tree->data, val);
        default:        return NULL;
    }
}

/** Get the successor value of val */
static int bst_next_val(Tree* tree, int val) {
    if (!tree->data) return 0;
    switch (tree->type) {
        case RB_TREE:   return rb_tree_next_val((RB_Tree*)tree->data, val);
        case AVL_TREE:  return avl_tree_next_val((AVL_Tree*)tree->data, val);
        case TREAP:     return treap_next_val((Treap*)tree->data, val);
        case SPLAY:     return splay_next_val((Splay*)tree->data, val);
        default:        return 0;
    }
}