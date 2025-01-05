// @brief Static Range Count Distinct in $O((n+q)\sqrt[3]{n})$
// https://codeforces.com/blog/entry/83248
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
using namespace std;
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define PROBLEM "https://judge.yosupo.jp/problem/static_range_count_distinct"
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
struct chash
{
    uint64_t operator()(int x) const noexcept
    {
        // splitmix64
        uint64_t z = (x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
};
const int B = 160, NM = 500000;
int f[NM / B + 2][NM / B + 2];
int pre[NM], nxt[NM];
int h[NM / B + 2];
void init()
{
    fill(nxt, nxt + NM, 1e9);
    fill(pre, pre + NM, -1e9);
}
int g(int x, int y)
{
    if (y < 0)
        return 0;
    int res = (x >= 0 ? f[x / B][y / B] : 0) + h[y / B];
    for (int z = max(0, (x / B) * B); z <= x; z++)
        res += nxt[z] < (y / B) * B;
    for (int z = max(0, (y / B) * B); z <= y; z++)
        res += pre[z] <= x;
    return res;
}
void Yoshi()
{
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &i : a)
        cin >> i;
    cc_hash_table<int, int, chash> idx;
    for (int i = 0; i < n; i++)
    {
        if (idx.find(a[i]) != idx.end())
            pre[i] = idx[a[i]], nxt[idx[a[i]]] = i;
        idx[a[i]] = i;
    }
    for (int i = 0; i < n; i++)
        if (pre[i] >= 0)
            f[pre[i] / B + 1][i / B + 1]++;
        else
            h[i / B + 1]++;
    partial_sum(h, h + NM / B + 2, h);
    for (int i = 1; i <= (n - 1) / B + 2; i++)
        for (int j = 1; j <= (n - 1) / B + 2; j++)
            f[i][j] += f[i][j - 1] + f[i - 1][j] - f[i - 1][j - 1];
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        r--;
        cout << g(l - 1, r) - g(l - 1, l - 1) << endl;
    }
}
signed main()
{
#ifndef yoshi_likes_e4
    ios::sync_with_stdio(0);
    cin.tie(0);
#endif
    init();
    int t = 1;
#if multitest
    cin >> t;
#endif
    while (t--)
        Yoshi();
}
