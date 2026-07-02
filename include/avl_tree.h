// avl_tree.h（公开头文件）
#ifndef AVL_TREE_H
#define AVL_TREE_H

// 声明 Node 结构体（不暴露内部细节）
typedef struct Node Node;

// 定义 AVLTree（包含根节点和哨兵）
typedef struct {
    Node* root;
    Node* nil;
    Node* nodes;
    int idx, MAX_NODES;
    int (*cmp)(int a, int b);   // 可选比较函数
} AVLTree;

// 公开 API
AVLTree* avl_create(int n);
void avl_insert(AVLTree* tree, int val);
void avl_delete(AVLTree* tree, int val);
Node* avl_find(AVLTree* tree, int val);
void avl_destroy(AVLTree* tree);
Node* avl_upper_bound(AVLTree* tree, int val);
Node* avl_lower_bound(AVLTree* tree, int val);
Node* avl_find_successor(AVLTree* tree, int val);
int avl_find_successor_val(AVLTree* tree, int val);
Node* avl_find_predecessor(AVLTree* tree, int val);
int avl_find_predecessor_val(AVLTree* tree, int val);
Node* avl_find_kth(AVLTree* tree, int k);
int avl_find_kth_val(AVLTree* tree, int val);
int avl_find_rank(AVLTree* tree, int val);


#endif