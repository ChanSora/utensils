#include <stdlib.h>
#include <stdio.h>
typedef struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int val;
    int size;
    int weight;
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

    while (node->weight > parent->weight && node->parent != NIL) {
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

static void inorder(Node* node, Node* NIL) {
    if (node == NIL) return;
    inorder(node->left, NIL);
    printf("In node %d\n", node->val);
    inorder(node->right, NIL);
}

Node node[2000010];
int idx;

static Node* allocate_node(int val, Node* NIL) {
    ++idx;
    node[idx].parent = NIL;
    node[idx].left = NIL;
    node[idx].right = NIL;
    node[idx].val = val;
    node[idx].size = 1;
    node[idx].weight = rand();
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
    NIL->weight = 0;

    root_ptr = NIL;

    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1, x; i <= n; i++) {
        scanf("%d", &x);
        insert(&root_ptr, allocate_node(x, NIL), NIL);
    }
    int ans = 0, last = 0;
    for (int i = 1, opt, x; i <= m; i++) {
        scanf("%d%d", &opt, &x);
        x = x ^ last;
        switch (opt) {
        case 1:
            insert(&root_ptr, allocate_node(x, NIL), NIL);
            break;
        case 2:
            erase(&root_ptr, x, NIL);
            break;
        case 3:
            last = rank(root_ptr, x, NIL);
            ans ^= last;
            break;
        case 4:
            last = kth(root_ptr, x, NIL)->val;
            ans ^= last;
            break;
        case 5:
            last = prev(root_ptr, x, NIL)->val;
            ans ^= last;
            break;
        case 6:
            last = upper_bound(root_ptr, x, NIL)->val;
            ans ^= last;
            break;
        default:
            break;
        }
    }
    printf("%d\n", ans);
    return 0;
}