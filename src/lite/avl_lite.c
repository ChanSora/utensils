#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    struct Node* left;
    struct Node* right;
    int val;
    int height;
    int size;
} Node;

const int MAXN = 1e6+10;
Node node[MAXN];
Node* NIL = &node[0];
Node* root_ptr = &node[0];

int idx;

inline int get_height(Node* node) {
    return (node) ? node->height : 0;
}

inline int get_size(Node* node) {
    return (node) ? node->size : 0;
}

void update_height(Node* node) {
    if (node->height == 0) return;
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);
    node->height = 1 + left_height > right_height ? left_height : right_height;
    node->size = 1 + get_size(node->left) + get_size(node->right);
}

Node* rotate_right(Node* pivot) {
    Node* new_root = pivot->left;
    Node* b = pivot->left->right;
    
    pivot->left = b;
    new_root->right = pivot;

    update_height(pivot);
    update_height(new_root);
    return new_root;
}

Node* rotate_left(Node* pivot) {
    Node* new_root = pivot->right;
    Node* b = pivot->right->left;

    pivot->right = b;
    new_root->left = pivot;
    
    update_height(pivot);
    update_height(new_root);
    return new_root;
}

int get_balance(Node* node) {
    if (node == NIL) return 0;
    return get_height(node->left) - get_height(node->right);
}

Node* rebalance(Node* pivot) {
    int balance = get_balance(pivot);
    if (abs(balance) <= 1) return pivot;
    if (balance > 0) { // balance > 1, L heavy
        if (get_balance(pivot->left) < 0) pivot->left = rotate_left(pivot->left); // LR
        return rotate_right(pivot);
    } else { // balance < -1, R heavy
        if (get_balance(pivot->right) > 0) pivot->right = rotate_right(pivot->right); // RL
        return rotate_left(pivot);
    }
}

Node* insert_val(Node* root, int val) {
    // printf("testing... root = %p \n", root);
    if (root == NIL) {
        // printf("in NIL...\n");
        ++idx;
        node[idx].val = val;
        node[idx].left = NIL;
        node[idx].right = NIL;
        node[idx].height = 1;
        node[idx].size = 1;
        // printf("return...\n");
        return &node[idx];
    }
    if (val < root->val) root->left = insert_val(root->left, val);
    else root->right = insert_val(root->right, val);
    
    // printf("done. update...\n");
    update_height(root);
    // printf("done. return...\n");
    return rebalance(root);
}

Node* find_min(Node* node) {
    if (node == NIL) return NIL;
    while (node->left != NIL) node = node->left;
    return node;
}

Node* delete_val(Node* root, int val) {
    if (root == NIL) return NIL;
    if (val > root->val) root->right = delete_val(root->right, val);
    else if (val < root->val) root->left = delete_val(root->left, val);
    else { // root->val == val
        if (root->left == NIL) return root->right;
        else if (root->right == NIL) return root->left;
        else {
            Node* successor = find_min(root->right);
            root->val = successor->val;
            root->right = delete_val(root->right, successor->val);
        }
    }
    update_height(root);
    return rebalance(root);
}

Node* find(Node* root, int val) {
    while (root != NIL) {
        if (val < root->val) root = root->left;
        else if (val > root->val) root = root->right;
        else return root;
    }
    return NIL;
}

Node* find_suc(Node* root, int val) {
    Node* suc = NIL;
    while (root != NIL) {
        if (root->val > val) {
            suc = root;        // 当前节点是一个候选答案
            root = root->left;    // 尝试找更小的满足条件的节点
        } else root = root->right;   // 当前节点太小，去右子树找
    }
    return suc;
}

Node* find_pre(Node* root, int val) {
    Node* pred = NIL;
    while (root != NIL) {
        if (root->val < val) {
            pred = root;              // 当前节点是候选前驱
            root = root->right;        // 找更大的
        } else root = root->left;         // 当前节点 >= val，去左子树找更小的候选
    }
    return pred;
}

int kth(Node* root, int k) {  // 1-indexed
    if (root == NIL) return 0;
    int left_size = (root->left == NIL) ? 0 : get_size(root->left);
    if (k <= left_size) return kth(root->left, k);
    if (k == left_size + 1) return root->val;
    return kth(root->right, k - left_size - 1);
}

int find_rank(Node* root, int val) {
    int rank = 0;
    while (root != NIL && root->height != 0) {
        if (val <= root->val) {
            root = root->left;
        } else {
            rank += get_size(root->left) + 1;
            root = root->right;
        }
    }
    return rank + 1;
}
void print(Node* root) {
    if (!root) return;
    if (root->height == 0) return;
    print(root->left);
    printf("%d\n", root->val);
    print(root->right);
}
int main() {
    freopen("P3369_9.in", "r", stdin);
    freopen("my_P3369_9_lite.out", "w", stdout);
    node[0] = (Node){&node[0], &node[0], 0, 0, 0};
    int n;
    scanf("%d", &n);
    for (int i = 1, opt, x; i <= n; i++) {
        scanf("%d%d", &opt, &x);
        // printf("---");
        switch (opt) {
        case 1:
            root_ptr = insert_val(root_ptr, x);
            break;
        case 2:
            root_ptr = delete_val(root_ptr, x);
            break;
        case 3:
            printf("3-%d\n", find_rank(root_ptr, x));
            break;
        case 4:
            printf("4-%d\n", kth(root_ptr, x));
            break;
        case 5:
            // printf("predecessor of %d: %d.\n", x, avl_find_predecessor_val(tree, x));
            printf("5-%d\n", find_pre(root_ptr, x)->val);
            break;
        case 6:
            // printf("successor of %d: %d.\n", x, avl_find_successor_val(tree, x));
            printf("6-%d\n", find_suc(root_ptr, x)->val);
            break;
        default:
            break;
        }
    }
    print(root_ptr);
    return 0;
}

