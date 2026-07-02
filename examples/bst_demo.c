#include "avl_tree.h"
#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    AVLTree* tree = avl_create(n);
    for (int i = 1, opt, x; i <= n; i++) {
        scanf("%d%d", &opt, &x);
        switch (opt) {
        case 1:
            avl_insert(tree, x);
            break;
        case 2:
            avl_delete(tree, x);
            break;
        case 3:
            // printf("Smaller than %d : totally %d.\n", x, avl_find_rank(tree, x));
            printf("%d\n", avl_find_rank(tree, x));
            break;
        case 4:
            // printf("the %dth number: %d.\n", x, avl_find_kth_val(tree, x));
            printf("%d\n", avl_find_kth_val(tree, x));
            break;
        case 5:
            // printf("predecessor of %d: %d.\n", x, avl_find_predecessor_val(tree, x));
            printf("%d\n", avl_find_predecessor_val(tree, x));
            break;
        case 6:
            // printf("successor of %d: %d.\n", x, avl_find_successor_val(tree, x));
            printf("%d\n", avl_find_successor_val(tree, x));
            break;
        default:
            break;
        }
    }
    avl_destroy(tree);
    return 0;
}