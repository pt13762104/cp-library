// @brief Matrix multiplication (with kernel)
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#pragma GCC optimize("O3,unroll-loops")
#include <chrono>
#include <iostream>
#include <random>
using namespace std;
using namespace chrono;
high_resolution_clock Clock;
mt19937_64 rng(steady_clock::now().time_since_epoch().count());
uint32_t a[1024][1024], b[1024][1024];
uint64_t c[1024][1024];
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m, k;
    cin >> n >> m >> k;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> a[i][j];
    for (int i = 0; i < m; i++)
        for (int j = 0; j < k; j++)
            cin >> b[i][j];
    auto t1 = Clock.now();
    uint64_t modmod8 = 8ULL * 998244353 * 998244353;
#define endl '\n'
#define B 4
    for (int i = 0; i < 1024; i += B)
        for (int k = 0; k < 1024; k++)
        {
            for (int j = 0; j < 1024; j += B)
                for (int n = 0; n < B; n++)
                    for (int m = 0; m < B; m++)
                        c[i + n][j + m] += (size_t)a[i + n][k] * b[k][j + m];
            if (k % 8 == 0)
                for (int n = 0; n < B; n++)
                    for (int j = 0; j < 1024; j++)
                        c[i + n][j] = min(c[i + n][j], c[i + n][j] - modmod8);
        }
    auto t2 = Clock.now();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
            cout << c[i][j] % 998244353 << " ";
        cout << endl;
    }
    cerr << "Time: " << duration_cast<milliseconds>(t2 - t1).count() << " ms" << endl;
    cerr << "GIOPS: " << (1024 * 1024 * 1024.0) / duration_cast<nanoseconds>(t2 - t1).count() << endl;
}
