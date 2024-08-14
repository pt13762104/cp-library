#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"
#include <bits/stdc++.h>
using namespace std;
#define int long long
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
const int MOD = 998244353;
int bpow(int n, int k)
{
    int res = 1;
    while (k)
    {
        if (k & 1)
        {
            res *= n;
            res %= MOD;
        }
        n *= n;
        n %= MOD;
        k >>= 1;
    }
    return res;
}
int inv(int i)
{
    return bpow(i, MOD - 2);
}
int us(int a, int b)
{
    int r = (a - b) % MOD;
    return r + (r < 0 ? MOD : 0);
}
pair<vector<vector<int>>, int> __ref(vector<vector<int>> x)
{
    int h = 0, k = 0;
    int sign = 1;
    while (h < x.size() && k < x[0].size())
    {
        // Any non-zero pivot row works
        int idx = h;
        while (idx < x.size() && x[idx][k] == 0)
            idx++;
        if (idx != x.size())
        {
            swap(x[h], x[idx]);
            if (idx != h)
            {
                sign *= (MOD - 1);
                sign %= MOD;
            }
            for (int i = h + 1; i < x.size(); i++)
            {
                int coeff = (x[i][k] * inv(x[h][k])) % MOD;
                for (int j = k; j < x[0].size(); j++)
                {
                    x[i][j] = us(x[i][j], coeff * x[h][j]);
                }
            }
            h++;
        }
        k++;
    }
    return {x, sign};
}
void init()
{
}
void Yoshi()
{
    int N;
    cin >> N;
    vector<vector<int>> raw(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> raw[i][j];
    // gaussian elimination
    auto [ref, sign] = __ref(raw);
    int determinant = 1;
    for (int i = 0; i < N; i++)
    {
        determinant *= ref[i][i];
        determinant %= MOD;
    }
    cout << (determinant * sign) % MOD << endl;
}
signed main()
{
#ifndef yoshi_likes_e4
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
#endif
    init();
    int t = 1;
#if multitest
    cin >> t;
#endif
    while (t--)
        Yoshi();
}