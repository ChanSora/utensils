#include <iostream>
#include <map>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

template <typename Map>
double test_erase_begin(Map& m) {
    auto start = steady_clock::now();
    while (!m.empty()) {
        m.erase(m.begin());
    }
    auto end = steady_clock::now();
    return duration<double>(end - start).count();
}

template <typename Map>
double test_erase_iterator(Map& m) {
    auto start = steady_clock::now();
    for (auto it = m.begin(); it != m.end(); ) {
        it = m.erase(it);
    }
    auto end = steady_clock::now();
    return duration<double>(end - start).count();
}

template <typename Map>
double test_clear(Map& m) {
    auto start = steady_clock::now();
    m.clear();
    auto end = steady_clock::now();
    return duration<double>(end - start).count();
}

int main() {
    const int N = 1000000;
    cout << "Testing std::map with " << N << " sequential erase operations\n";
    cout << "===========================================================\n";

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