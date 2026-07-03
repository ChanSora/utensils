#include<iostream>
#include<vector>
const int maxn = 1e5 + 10, inf = 2147483647;
int root, idx;
struct node {
    int s[2], val, size, cnt, fa;
    void init(int v_, int fa_) {
        val = v_, fa = fa_;
        size = cnt = 1;
    }
} t[maxn];
void pushup(int x) {
    t[x].size = t[t[x].s[0]].size + t[t[x].s[1]].size + t[x].cnt;
}
void rotate(int x) {
    int y = t[x].fa, z = t[y].fa;
    bool k = (t[y].s[1] == x);
    t[y].s[k] = t[x].s[!k];
    t[t[x].s[!k]].fa = y;
    t[x].s[!k] = y;
    t[y].fa = x;
    t[z].s[y == t[z].s[1]] = x;
    t[x].fa = z; 
    pushup(y); pushup(x);
}
void splay(int x, int k) {
    while (k != t[x].fa) {
        int y = t[x].fa, z = t[y].fa;
        if (z != k) (t[z].s[0] == y) ^ (t[y].s[0] == x)
            ? rotate(x) : rotate(y);
        rotate(x);
        // std::cout << t[x].fa << '\n';
    }
    if (k == 0) root = x;
}
void insert(int val) {
    int x = root, fa = 0;
    while (x && t[x].val != val) 
        fa = x, x = t[x].s[val > t[x].val];
    if (x) t[x].cnt++;
    else {
        x = ++idx;
        if (fa) t[fa].s[val > t[fa].val] = x;
        t[x].init(val, fa);
    }
    splay(x, 0);
}
void find(int val) {
    int x = root;
    while(t[x].s[val > t[x].val] && t[x].val != val)
        x = t[x].s[val > t[x].val];
    splay(x, 0);
}
int get_pre(int val) {
    find(val);
    int x = root;
    if (t[x].val < val) return x;
    x = t[x].s[0];
    while (t[x].s[1]) x = t[x].s[1];
    splay(x, 0);
    return x;
}
int get_suc(int val) {
    find(val);
    int x = root;
    if (t[x].val > val) return x;
    x = t[x].s[1];
    while (t[x].s[0]) x = t[x].s[0];
    splay(x, 0);
    return x;
}
void del(int val) {
    int pre = get_pre(val);
    int suc = get_suc(val);
    splay(pre, 0); splay(suc, pre);
    int d = t[suc].s[0];
    if (t[d].cnt > 1) t[d].cnt--, splay(d, 0);
    else t[suc].s[0] = 0, splay(suc, 0);
}
int get_rank(int val) {
    insert(val);
    int res = t[t[root].s[0]].size;
    del(val);
    return res;
}
int get_val(int k) {
    int x = root;
    while (true) {
        int y = t[x].s[0];
        if (t[y].size + t[x].cnt < k)
            k -= t[y].size + t[x].cnt, 
            x = t[x].s[1];
        else if (t[y].size >= k) x = y;
        else break;
    }
    splay(x, 0);
    return t[x].val;
}
void print(int x) {
    if (t[x].s[0]) print(t[x].s[0]);
    std::cout << t[x].val << '\n';
    if (t[x].s[1]) print(t[x].s[1]);
}
int main() {
    freopen("P3369_9.in", "r", stdin);
    freopen("check_P3369_9_37.out", "w", stdout);
    // std::ios::sync_with_stdio(false);
    // std::cin.tie(0);
    
    int n; std::cin >> n;
    insert(inf); insert(-inf);
    for (int i = 1; i <= 37; i++) {
        int opt, x; std::cin >> opt >> x;
        if (opt == 1) insert(x);
        else if (opt == 2) del(x);
        else if (opt == 3) std::cout << get_rank(x) << '\n';
        else if (opt == 4) std::cout << get_val(x + 1) << '\n';
        else if (opt == 5) std::cout << t[get_pre(x)].val << '\n';
        else if (opt == 6) std::cout << t[get_suc(x)].val << '\n';
        // print(root);
    }
    return 0;
}