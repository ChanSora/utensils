/**
 * @file bst.h
 * @brief Polymorphic Balanced Binary Search Tree interface
 *
 * This header defines a unified, type-erased Tree interface backed by a
 * vtable of function pointers.  The same `Tree` object can dispatch to
 * any of the four concrete balanced BST implementations:
 *
 *   - Red-Black Tree  (RB_TREE)
 *   - AVL Tree        (AVL_TREE)
 *   - Treap           (TREAP)
 *   - Splay Tree      (SPLAY)
 *
 * Usage:
 *   Tree t = tree_init(AVL_TREE);
 *   t.create(&t, 10000);
 *   t.insert(&t, 42);
 *   int r = t.rank(&t, 42);
 *   t.destroy(&t);
 */

/** Supported concrete tree implementations */
typedef enum TreeType {
    RB_TREE,     ///< Red-Black Tree
    AVL_TREE,    ///< AVL Tree
    TREAP,       ///< Treap (Tree + Heap)
    SPLAY        ///< Splay Tree
} TreeType;

/**
 * Polymorphic balanced BST handle.
 *
 * All pointer fields are function pointers implementing the vtable.
 * `data` holds a type-erased pointer to the concrete tree (RB_Tree*,
 * AVL_Tree*, etc.).  Call tree_init() to set up the vtable, then
 * create() to allocate the concrete tree.
 */
typedef struct Tree {
    TreeType type;       ///< Backend tree type

    void* data;          ///< Opaque pointer to the concrete tree instance

    void (*create)(struct Tree* self, int capacity);
    void (*destroy)(struct Tree* self);
    void (*insert)(struct Tree* self, int val);
    void (*erase)(struct Tree* self, int val);
    void* (*find)(struct Tree* self, int val);
    int (*find_val)(struct Tree* self, int val);
    int (*rank)(struct Tree* self, int val);
    void* (*kth)(struct Tree* self, int k);
    int (*kth_val)(struct Tree* self, int k);
    void* (*prev)(struct Tree* self, int val);
    int (*prev_val)(struct Tree* self, int val);
    void* (*lower_bound)(struct Tree* self, int val);
    void* (*upper_bound)(struct Tree* self, int val);
    void* (*next)(struct Tree* self, int val);
    int (*next_val)(struct Tree* self, int val);
} Tree;

/**
 * @brief Initialise a Tree with the vtable for the chosen backend.
 * @param type One of RB_TREE, AVL_TREE, TREAP, or SPLAY
 * @return A Tree whose function pointers dispatch to the selected
 *         implementation.  Call create() before any other operations.
 */
Tree tree_init(TreeType type);

