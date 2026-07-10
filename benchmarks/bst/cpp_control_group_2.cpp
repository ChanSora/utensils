#include <iostream>
#include <map>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
using namespace chrono;

double test_random_erase(map<int, int>& m, const vector<int>& del_order) {
    auto start = steady_clock::now();
    for (int key : del_order) {
        m.erase(key);
    }
    auto end = steady_clock::now();
    return duration<double>(end - start).count();
}

int main() {
    const int N = 1000000;
    cout << "Testing std::map random erase with N = " << N << "\n";
    cout << "===========================================\n";

    vector<int> keys(N);
    iota(keys.begin(), keys.end(), 1);

    shuffle(keys.begin(), keys.end(), mt19937(42));

    cout << "Inserting " << N << " elements in shuffled order...\n";
    map<int, int> m;
    for (int key : keys) {
        m[key] = key;
    }
    cout << "Insertion done. size() = " << m.size() << "\n";

    vector<int> del_order = keys; // 复制一份
    shuffle(del_order.begin(), del_order.end(), mt19937(12345));

    cout << "Starting random erase...\n";
    double t = test_random_erase(m, del_order);
    cout << "Random erase time: " << fixed << setprecision(4) << t << " sec\n";

    cout << "Final size: " << m.size() << " (should be 0)\n";

    return 0;
}