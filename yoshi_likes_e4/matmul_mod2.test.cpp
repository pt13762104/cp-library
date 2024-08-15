// @brief Matrix multiplication (mod 2)
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product_mod_2"
#pragma GCC optimize("O3,unroll-loops")
#include <bitset>
#include <iostream>
using namespace std;
#define endl '\n'
#define debug(x) cerr << #x << " = " << x << endl;
unsigned long long a[4096][64];
unsigned long long b[4096][64];
int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m, k;
    cin >> n >> m >> k;
    int clk = clock();
    string s;
    for (int i = 0; i < n; i++)
    {
        cin >> s;
        for (int j = 0; j < m; j++)
            if (s[j] - 48)
                a[i][j >> 6] |= 1ULL << (j & 63);
    }
    for (int i = 0; i < m; i++)
    {
        cin >> s;
        for (int j = 0; j < k; j++)
            if (s[j] - 48)
                b[j][i >> 6] |= 1ULL << (i & 63);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            int r = 0;
            for (int k = 0; k < 64; k++)
                r += __builtin_popcountll(a[i][k] & b[j][k]);
            s[j] = (r & 1) + 48;
        }
        cout << s << endl;
    }
}