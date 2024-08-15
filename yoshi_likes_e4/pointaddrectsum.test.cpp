// @brief Point Add Rectangle Sum
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
#define debug(x) cerr << #x << " = " << x << endl;
const int FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
struct custom_hash_table
{
    int LG = 0, count = 0;
    size_t hash(uint32_t x)
    {
        // shifting by 64 bits is undefined
        return (LG ? ((x + FIXED_RANDOM) * 11995408973635179863ULL) >> (64 - LG) : 0);
    }
    vector<vector<pair<uint32_t, uint64_t>>> buckets = {{}};
    void rehash()
    {
        auto old_buckets = buckets;
        LG += 3;
        buckets.resize(1 << LG);
        for (int i = 0; i < old_buckets.size(); i++)
            buckets[i] = {};
        for (auto &i : old_buckets)
            for (auto &j : i)
                buckets[hash(j.first)].push_back(j);
    }
    void add(uint32_t k, uint32_t v)
    {
        size_t hs = hash(k);
        for (auto &i : buckets[hs])
            if (i.first == k)
            {
                i.second += v;
                return;
            }
        buckets[hs].push_back({k, v});
        count++;
        if (count > 7 * (1 << LG))
            rehash();
    };
    uint64_t query(uint32_t k)
    {
        size_t hs = hash(k);
        for (auto &i : buckets[hs])
            if (i.first == k)
                return i.second;
        return 0ul;
    };
};
custom_hash_table fw_tree[200001];
int MAX = 200000;
uint64_t getpfs(int i, int j)
{
    int k = j;
    uint64_t res = 0;
    for (; i; i -= (i & (-i)))
        for (j = k; j; j -= (j & (-j)))
            res += fw_tree[i].query(j);
    return res;
}
void update(int i, int j, int w)
{
    int k = j;
    for (; i <= MAX; i += (i & (-i)))
        for (j = k; j <= MAX; j += (j & (-j)))
            fw_tree[i].add(j, w);
}
uint64_t getsum(int l, int r, int d, int u)
{
    return getpfs(r, u) + getpfs(l, d) - getpfs(l, u) - getpfs(r, d);
}
void init()
{
}
struct Query
{
    int t, l, r, d, u, x, y, w;
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
        queries.push_back({0, 0, 0, 0, 0, x, y, w});
        xv.push_back(x);
        yv.push_back(y);
    }
    while (q--)
    {
        int t;
        cin >> t;
        if (t)
        {
            int l, r, d, u;
            cin >> l >> d >> r >> u;
            queries.push_back({t, l, r, d, u, 0, 0, 0});
        }
        else
        {
            int x, y, w;
            cin >> x >> y >> w;
            queries.push_back({t, 0, 0, 0, 0, x, y, w});
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
            cout << getsum(lower_bound(xv.begin(), itx, qu.l) - xv.begin(),
                           lower_bound(xv.begin(), itx, qu.r) - xv.begin(),
                           lower_bound(yv.begin(), ity, qu.d) - yv.begin(),
                           lower_bound(yv.begin(), ity, qu.u) - yv.begin())
                 << endl;
        else
            update(upper_bound(xv.begin(), itx, qu.x) - xv.begin(), upper_bound(yv.begin(), ity, qu.y) - yv.begin(),
                   qu.w);
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