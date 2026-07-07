#include <iostream>
#include <map>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
using namespace chrono;

// 测试随机删除（按值 erase）
double test_random_erase(map<int, int>& m, const vector<int>& del_order) {
    auto start = steady_clock::now();
    for (int key : del_order) {
        m.erase(key);
    }
    auto end = steady_clock::now();
    return duration<double>(end - start).count();
}

int main() {
    const int N = 10000000; // 1e7，可调整为 1e6 等
    cout << "Testing std::map random erase with N = " << N << "\n";
    cout << "===========================================\n";

    // 1. 生成键序列 1..N
    vector<int> keys(N);
    iota(keys.begin(), keys.end(), 1);

    // 2. 打乱插入顺序（固定种子，可复现）
    shuffle(keys.begin(), keys.end(), mt19937(42));

    // 3. 按打乱顺序插入 map
    cout << "Inserting " << N << " elements in shuffled order...\n";
    map<int, int> m;
    for (int key : keys) {
        m[key] = key;
    }
    cout << "Insertion done. size() = " << m.size() << "\n";

    // 4. 准备删除顺序：再次打乱（独立随机顺序）
    //    为了公平，使用不同的种子（或者直接复用 keys 并再次 shuffle）
    vector<int> del_order = keys; // 复制一份
    shuffle(del_order.begin(), del_order.end(), mt19937(12345));

    // 5. 执行随机删除并计时
    cout << "Starting random erase...\n";
    double t = test_random_erase(m, del_order);
    cout << "Random erase time: " << fixed << setprecision(4) << t << " sec\n";

    // 6. 校验是否全部删除
    cout << "Final size: " << m.size() << " (should be 0)\n";

    return 0;
}