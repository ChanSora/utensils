#include<iostream>
const int maxn = 1100010, inf = (1 << 30) + 1;
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
        if (k <= t[y].size) x = y;
        else if (k <= t[y].size + t[x].cnt) break;
        else k -= t[y].size + t[x].cnt, x = t[x].s[1];
    }
    splay(x, 0);
    return t[x].val;
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    freopen("P6136_2.in", "r", stdin);
    freopen("check_splay.out", "w", stdout);
    int n, m, last = 0, res = 0; std::cin >> n >> m;
    insert(inf); insert(-inf);
    for (int i = 1; i <= n; i++) {
        int x; std::cin >> x;
        insert(x);
    }
    for (int i = 1; i <= m; i++) {
        int opt, x; std::cin >> opt >> x;
        x ^= last;
        if (opt == 1) insert(x);
        else if (opt == 2) del(x);
        else if (opt == 3) res ^= (last = get_rank(x));
        else if (opt == 4) res ^= (last = get_val(x + 1));
        else if (opt == 5) res ^= (last = t[get_pre(x)].val);
        else if (opt == 6) res ^= (last = t[get_suc(x)].val);
    }
    std::cout << res << '\n';
    return 0;
}