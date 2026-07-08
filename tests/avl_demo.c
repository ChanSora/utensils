#include <stdlib.h>
#include <stdio.h>
inline static int max(int a, int b) {return a > b ? a : b;}
// inline static int abs(int a) {return a > 0 ? a : - a;}

struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int val;
    int height;
    int size;
};

typedef struct Node Node;

// 定义 AVLTree（包含根节点和哨兵）
typedef struct {
    Node* root;
    Node* nil;
    Node* nodes;
    int idx, capacity;
    int (*cmp)(int a, int b);   // 可选比较函数
} AVL_Tree;

// 公开 API
AVL_Tree* avl_create(int n);
void avl_insert(AVL_Tree* tree, int val);
void avl_delete(AVL_Tree* tree, int val);
Node* avl_tree_find(AVL_Tree* tree, int val);
void avl_destroy(AVL_Tree* tree);
Node* avl_tree_upper_bound(AVL_Tree* tree, int val);
Node* avl_tree_lower_bound(AVL_Tree* tree, int val);
Node* avl_tree_next(AVL_Tree* tree, int val);
int avl_next_val(AVL_Tree* tree, int val);
Node* avl_tree_prev(AVL_Tree* tree, int val);
int avl_tree_prev_val(AVL_Tree* tree, int val);
Node* avl_tree_kth(AVL_Tree* tree, int k);
int avl_tree_kth_val(AVL_Tree* tree, int val);
int avl_tree_rank(AVL_Tree* tree, int val);

// Node 结构体定义在 .c 文件（外部不可见）
inline static int get_height(Node* NIL, Node* node) {
    return (node == NIL) ? 0 : node->height;
}

static void update_height(Node* NIL, Node* node) {
    if (node == NIL) return;
    int left_height = get_height(NIL, node->left);
    int right_height = get_height(NIL, node->right);
    node->height = 1 + max(left_height, right_height);
    node->size = 1 + node->left->size + node->right->size;
}

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

static int get_balance(Node* NIL, Node* node) {
    if (node == NIL) return 0;
    return get_height(NIL, node->left) - get_height(NIL, node->right);
}

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

static void insert(Node** root_ptr, Node* NIL, Node* root, Node* node) {
    // printf("current root:%d\n", root->val);
    // printf("left: %d, right: %d.\n", root->left->val, root->right->val);
    // printf("node val:%d\n", node->val);
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

static Node* find_min(Node* NIL, Node* node) {
    if (node == NIL) return NIL;
    while (node->left != NIL) node = node->left;
    return node;
}

static Node* find_max(Node* NIL, Node* node) {
    if (node == NIL) return NIL;
    while (node->right != NIL) node = node->right;
    return node;
}

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

static Node* find(Node* root, Node* NIL, int val) {
    while (root != NIL) {
        if (val < root->val) root = root->left;
        else if (val > root->val) root = root->right;
        else return root;
    }
    return NIL;
}

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

static Node* kth(Node* NIL, Node* root, int k) {  // 1-indexed
    if (root == NIL) return NIL;
    int left_size = (root->left == NIL) ? 0 : root->left->size;
    if (k <= left_size) return kth(NIL, root->left, k);
    if (k == left_size + 1) return root;
    return kth(NIL, root->right, k - left_size - 1);
}

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

AVL_Tree* avl_create(int n) {
    if (n <= 0) return NULL;

    // 1. 分配树结构体
    AVL_Tree* tree = (AVL_Tree*)malloc(sizeof(AVL_Tree));
    if (!tree) return NULL;

    // 2. 分配节点池（索引 0 留给哨兵）
    tree->nodes = (Node*)malloc((n + 1) * sizeof(Node));
    if (!tree->nodes) {
        free(tree);
        return NULL;
    }

    // 3. 初始化
    tree->idx = 0;
    tree->capacity = n;

    // 4. 初始化哨兵节点（索引 0）
    tree->nil = &tree->nodes[0];
    tree->nil->parent = tree->nil;
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil;
    tree->nil->height = 0;
    tree->nil->val = 0;
    tree->nil->size = 0;

    // 5. 根节点初始指向哨兵（空树）
    tree->root = tree->nil;

    return tree;
}

void avl_insert(AVL_Tree* tree, int val) {
    // 防止越界
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

void avl_delete(AVL_Tree* tree, int val) {
    if (tree->root == tree->nil) return;
    delete_val(&tree->root, tree->nil, tree->root, val);
}

Node* avl_tree_find(AVL_Tree* tree, int val) {
    return find(tree->root, tree->nil, val);
}

void avl_destroy(AVL_Tree* tree) {
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

int avl_next_val(AVL_Tree* tree, int val) {
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

int avl_tree_kth_val(AVL_Tree* tree, int val) {
    return avl_tree_kth(tree, val)->val;
}

int avl_tree_rank(AVL_Tree* tree, int val) {
    // avl_insert(tree, val);
    return find_rank(tree->nil, tree->root, val);
    // int res = find_rank(tree->nil, tree->root, val);
    // avl_delete(tree, val);
    // return res;
}

int main() {
    // freopen("P3369_9.in", "r", stdin);
    // freopen("my_P3369_9.out", "w", stdout);
    int n, m;
    scanf("%d%d", &n, &m);
    AVL_Tree* tree = avl_create(n + m);
    for (int i = 1, x; i <= n; i++) {
        scanf("%d", &x);
        avl_insert(tree, x);
    }
    int ans = 0, last = 0;
    for (int i = 1, opt, x; i <= m; i++) {
        scanf("%d%d", &opt, &x);
        x = x ^ last;
        // printf("---");
        switch (opt) {
        case 1:
            avl_insert(tree, x);
            break;
        case 2:
            avl_delete(tree, x);
            break;
        case 3:
            // printf("Smaller than %d : totally %d.\n", x, avl_tree_rank(tree, x));
            // printf("%d\n", avl_tree_rank(tree, x));
            last = avl_tree_rank(tree, x);
            ans ^= last;
            break;
        case 4:
            // printf("the %dth number: %d.\n", x, avl_tree_kth_val(tree, x));
            // printf("%d\n", avl_tree_kth_val(tree, x));
            last = avl_tree_kth_val(tree, x);
            ans ^= last;
            break;
        case 5:
            // printf("predecessor of %d: %d.\n", x, avl_tree_prev_val(tree, x));
            // printf("%d\n", avl_tree_prev_val(tree, x));
            last = avl_tree_prev_val(tree, x);
            ans ^= last;
            break;
        case 6:
            // printf("successor of %d: %d.\n", x, avl_next_val(tree, x));
            // printf("%d\n", avl_next_val(tree, x));
            last = avl_next_val(tree, x);
            ans ^= last;
            break;
        default:
            break;
        }
    }
    avl_destroy(tree);
    printf("%d\n", ans);
    return 0;
}
