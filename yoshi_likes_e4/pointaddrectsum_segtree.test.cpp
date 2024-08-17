// @brief Point Add Rectangle Sum using dynamic Segment Tree
#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define int uint32_t
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define multitest 0
struct dynamic_segtree
{
    uint64_t v = 0;
    dynamic_segtree *l = 0, *r = 0;
};
uint64_t get(dynamic_segtree *i, int u, int v, int l, int r)
{
    if (!i || v < l || r < u)
        return 0ul;
    if (l <= u && v <= r)
        return i->v;
    else
    {
        int mid = (u + v) >> 1;
        return get(i->l, u, mid, l, r) + get(i->r, mid + 1, v, l, r);
    }
}
void update(dynamic_segtree *&i, int u, int v, int p, int x)
{
    if (!i)
        i = new dynamic_segtree;
    if (u != v)
    {
        int mid = (u + v) >> 1;
        if (p <= mid)
            update(i->l, u, mid, p, x);
        else
            update(i->r, mid + 1, v, p, x);
    }
    i->v += x;
}
dynamic_segtree *fw_tree[200001];
int MAX = 200000;
uint64_t getsum(int i, int j, int k)
{
    uint64_t res = 0;
    for (; i; i -= (i & (-i)))
        res += get(fw_tree[i], 1, MAX, j + 1, k);
    return res;
}
void update(int i, int j, int w)
{
    for (; i <= MAX; i += (i & (-i)))
        update(fw_tree[i], 1, MAX, j, w);
}
uint64_t getsum(int l, int r, int d, int u)
{
    return getsum(r, d, u) - getsum(l, d, u);
}
void init()
{
}
struct Query
{
    bool t;
    int a1 = 0, a2 = 0, a3 = 0, a4 = 0;
};
void Yoshi()
{
    int n, q;
    cin >> n >> q;
    vector<int> xv, yv;
    vector<Query> queries;
    while (n--)
    {
        int x, y, w;
        cin >> x >> y >> w;
        queries.push_back({0, x, y, w});
        xv.push_back(x);
        yv.push_back(y);
    }
    while (q--)
    {
        bool t;
        cin >> t;
        if (t)
        {
            int l, r, d, u;
            cin >> l >> d >> r >> u;
            queries.push_back({t, l, r, d, u});
        }
        else
        {
            int x, y, w;
            cin >> x >> y >> w;
            queries.push_back({t, x, y, w});
            xv.push_back(x);
            yv.push_back(y);
        }
    }
    sort(xv.begin(), xv.end());
    sort(yv.begin(), yv.end());
    auto itx = unique(xv.begin(), xv.end());
    auto ity = unique(yv.begin(), yv.end());
    for (auto &qu : queries)
    {
        if (qu.t)
            cout << getsum(lower_bound(xv.begin(), itx, qu.a1) - xv.begin(),
                           lower_bound(xv.begin(), itx, qu.a2) - xv.begin(),
                           lower_bound(yv.begin(), ity, qu.a3) - yv.begin(),
                           lower_bound(yv.begin(), ity, qu.a4) - yv.begin())
                 << endl;
        else
            update(upper_bound(xv.begin(), itx, qu.a1) - xv.begin(), upper_bound(yv.begin(), ity, qu.a2) - yv.begin(),
                   qu.a3);
    }
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