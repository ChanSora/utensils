#include "avl_tree.h"
#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    AVL_Tree* tree = avl_tree_create(n);
    for (int i = 1, opt, x; i <= n; i++) {
        scanf("%d%d", &opt, &x);
        switch (opt) {
        case 1:
            avl_tree_insert(tree, x);
            break;
        case 2:
            avl_tree_delete(tree, x);
            break;
        case 3:
            // printf("Smaller than %d : totally %d.\n", x, avl_tree_rank(tree, x));
            printf("%d\n", avl_tree_rank(tree, x));
            break;
        case 4:
            // printf("the %dth number: %d.\n", x, avl_tree_kth_val(tree, x));
            printf("%d\n", avl_tree_kth_val(tree, x));
            break;
        case 5:
            // printf("predecessor of %d: %d.\n", x, avl_tree_prev_val(tree, x));
            printf("%d\n", avl_tree_prev_val(tree, x));
            break;
        case 6:
            // printf("successor of %d: %d.\n", x, avl_next_val(tree, x));
            printf("%d\n", avl_next_val(tree, x));
            break;
        default:
            break;
        }
    }
    avl_tree_destroy(tree);
    return 0;
}