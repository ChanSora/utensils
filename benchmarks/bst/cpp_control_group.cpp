#include <iostream>
#include <map>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// 测试1：循环 erase(begin()) —— 最直接的顺序删除
template <typename Map>
double test_erase_begin(Map& m) {
    auto start = steady_clock::now();
    while (!m.empty()) {
        m.erase(m.begin());
    }
    auto end = steady_clock::now();
    return duration<double>(end - start).count();
}

// 测试2：迭代器逐个删除（返回下一个迭代器）
template <typename Map>
double test_erase_iterator(Map& m) {
    auto start = steady_clock::now();
    for (auto it = m.begin(); it != m.end(); ) {
        it = m.erase(it);   // C++11 起 erase 返回下一个迭代器
    }
    auto end = steady_clock::now();
    return duration<double>(end - start).count();
}

// 测试3：clear() —— 一次性清空
template <typename Map>
double test_clear(Map& m) {
    auto start = steady_clock::now();
    m.clear();
    auto end = steady_clock::now();
    return duration<double>(end - start).count();
}

int main() {
    const int N = 10000000;   // 测试 1e6 数据，若想测 1e7 可改，但耗时较长
    cout << "Testing std::map with " << N << " sequential erase operations\n";
    cout << "===========================================================\n";

    // 准备数据：顺序插入 1..N
    {
        map<int, int> m;
        for (int i = 1; i <= N; ++i) m[i] = i;
        double t = test_erase_begin(m);
        cout << "erase(begin())  : " << fixed << setprecision(4) << t << " sec\n";
    }

    {
        map<int, int> m;
        for (int i = 1; i <= N; ++i) m[i] = i;
        double t = test_erase_iterator(m);
        cout << "erase(iterator) : " << fixed << setprecision(4) << t << " sec\n";
    }

    {
        map<int, int> m;
        for (int i = 1; i <= N; ++i) m[i] = i;
        double t = test_clear(m);
        cout << "clear()         : " << fixed << setprecision(4) << t << " sec\n";
    }

    return 0;
}