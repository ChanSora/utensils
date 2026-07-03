#ifndef AVL_LITE_H
#define AVL_LITE_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAXN 100010

typedef struct Node {
    struct Node* left;
    struct Node* right;
    int val;
    int height;
    int size;
} Node;

extern Node* NIL;
extern Node* root_ptr;

int get_height(Node* node);
int get_size(Node* node);
void update_height(Node* node);
Node* rotate_right(Node* pivot);
Node* rotate_left(Node* pivot);
int get_balance(Node* node);
Node* rebalance(Node* pivot);
Node* insert_val(Node* root, int val);
Node* find_min(Node* node);
Node* delete_val(Node* root, int val);
Node* find(Node* root, int val);
Node* find_suc(Node* root, int val);
Node* find_pre(Node* root, int val);
int kth(Node* root, int k);
int find_rank(Node* root, int val);
void print(Node* root);

#ifdef __cplusplus
}
#endif

#endif // AVL_LITE_H
