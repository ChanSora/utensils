#include <utensils/tree.h>
#include <stdio.h>

int main() {
    Tree t = tree_init(TREAP);
    t.create(&t, 1000);
    
    t.insert(&t, 50);
    t.insert(&t, 30);
    t.insert(&t, 70);
    
    void* node = t.find(&t, 30);
    printf("Found node: %p\n", node);
    
    int rank = t.rank(&t, 50);
    printf("Rank of 50: %d\n", rank);
    
    t.destroy(&t);
    return 0;
}