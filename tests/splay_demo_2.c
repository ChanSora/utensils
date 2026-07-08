#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int val;
    int size;
} Node;

// 快读优化
// 极致性能：基于 fread 的缓冲区快读
int gc() {
    // 4MB 缓冲区，一次性将大块数据读入内存，避免频繁 I/O
    static char buf[1 << 22], *p1 = buf, *p2 = buf;
    return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 1 << 22, stdin), p1 == p2) ? EOF : *p1++;
}

int read() {
    int x = 0, f = 1;
    int ch = gc(); // 必须用 int 接收，准确捕获 EOF
    
    // 核心改进：加入 ch != EOF 判定，防止文件末尾死循环
    while ((ch < '0' || ch > '9') && ch != EOF) {
        if (ch == '-') f = -1;
        ch = gc();
    }
    
    if (ch == EOF) return 0; // 如果触底文件末尾，直接安全退出
    
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + (ch - '0');
        ch = gc();
    }
    return x * f;
}

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
    else if (node == parent->left) parent->left = new_root;
    else parent->right = new_root;

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
    else if (node == parent->left) parent->left = new_root;
    else parent->right = new_root;
    
    update_size(node, NIL);
    update_size(new_root, NIL);
}

static void splay(Node** root_ptr, Node* node, Node* NIL) {
    if (node == NIL || node == *root_ptr) return;
    while (node->parent != NIL) {
        Node* parent = node->parent;
        Node* grandparent = parent->parent;
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
    }
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
        // 优化点 2：移除了 cur->size++ 减小常数开销
        if (node->val < cur->val) cur = cur->left;
        else cur = cur->right;
    }

    node->parent = parent;
    if (node->val < parent->val) parent->left = node;
    else parent->right = node;

    splay(root_ptr, node, NIL);
}

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
        if (parent != NIL) splay(root_ptr, parent, NIL);
        return;
    }
    splay(root_ptr, cur, NIL);

    if (cur->right == NIL) {
        *root_ptr = cur->left;
        if (cur->left != NIL) cur->left->parent = NIL;
        return;
    }

    Node* successor = cur->right;
    while (successor->left != NIL) successor = successor->left;

    splay(root_ptr, successor, NIL);

    successor->left = cur->left;
    if (cur->left != NIL) cur->left->parent = successor;

    update_size(successor, NIL);
}

static int rank(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* parent = NIL;
    int rank = 0;
    while (node != NIL) {
        parent = node;
        if (val <= node->val) node = node->left;
        else rank += node->left->size + 1, node = node->right;
    }
    if (parent != NIL) splay(root_ptr, parent, NIL);
    return rank + 1;
} 

static Node* kth(Node** root_ptr, int k, Node* NIL) {
    Node* node = *root_ptr;
    while (node != NIL) {
        if (node->left->size >= k) node = node->left;
        else if (node->left->size == k - 1) {
            splay(root_ptr, node, NIL);
            return node;
        }
        else k -= node->left->size + 1, node = node->right;
    }
    return NIL;
}

static Node* prev(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* prev = NIL;
    Node* last = NIL;
    while (node != NIL) {
        last = node;
        if (node->val < val) prev = node, node = node->right;
        else node = node->left;
    }
    // 优化点 1：未命中时补回对 last 的 splay
    if (prev != NIL) splay(root_ptr, prev, NIL);
    else if (last != NIL) splay(root_ptr, last, NIL);
    return prev;
}

static Node* lower_bound(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* next = NIL;
    Node* last = NIL;
    while (node != NIL) {
        last = node;
        if (node->val >= val) next = node, node = node->left;
        else node = node->right;
    }
    // 优化点 1：未命中时补回对 last 的 splay
    if (next != NIL) splay(root_ptr, next, NIL);
    else if (last != NIL) splay(root_ptr, last, NIL);
    return next;
}

static Node* upper_bound(Node** root_ptr, int val, Node* NIL) {
    Node* node = *root_ptr;
    Node* next = NIL;
    Node* last = NIL;
    while (node != NIL) {
        last = node;
        if (node->val > val) next = node, node = node->left;
        else node = node->right;
    }
    // 优化点 1：未命中时补回对 last 的 splay
    if (next != NIL) splay(root_ptr, next, NIL);
    else if (last != NIL) splay(root_ptr, last, NIL);
    return next;
}

// 稍微开大数组，防止极端的满插入数据发生越界
Node node[1200005];
int idx;

static Node* allocate_node(int val, Node* NIL) {
    ++idx;
    node[idx].parent = NIL;
    node[idx].left = NIL;
    node[idx].right = NIL;
    node[idx].val = val;
    node[idx].size = 1;
    return &node[idx];
}

int main() {
    Node nil_node = node[0];
    Node* NIL = &nil_node;
    Node* root_ptr;
    
    NIL->parent = NIL;
    NIL->left = NIL;
    NIL->right = NIL;
    NIL->size = 0;
    NIL->val = 0;

    root_ptr = NIL;

    int n, m;
    n = read(); m = read();
    for (int i = 1, x; i <= n; i++) {
        x = read();
        insert(&root_ptr, allocate_node(x, NIL), NIL);
    }
    int ans = 0, last = 0;
    for (int i = 1, opt, x; i <= m; i++) {
        opt = read(); x = read();
        x = x ^ last;
        switch (opt) {
        case 1:
            insert(&root_ptr, allocate_node(x, NIL), NIL);
            break;
        case 2:
            erase(&root_ptr, x, NIL);
            break;
        case 3:
            last = rank(&root_ptr, x, NIL);
            ans ^= last;
            break;
        case 4:
            last = kth(&root_ptr, x, NIL)->val;
            ans ^= last;
            break;
        case 5:
            last = prev(&root_ptr, x, NIL)->val;
            ans ^= last;
            break;
        case 6:
            last = upper_bound(&root_ptr, x, NIL)->val;
            ans ^= last;
            break;
        default:
            break;
        }
    }
    printf("%d\n", ans);
    return 0;
}