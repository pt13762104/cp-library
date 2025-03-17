// @brief Line Add Get Min
#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"
#include <bits/stdc++.h>
using namespace std;
#define int long long
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
vector<int> iota0(400000);
void init()
{
    iota(iota0.begin(), iota0.end(), 0);
}
const int B = 2400;
struct line
{
    int32_t m;
    long long c;
    long long eval(long long x)
    {
        return m * x + c;
    }
    bool operator<(const line &x) const
    {
        if (m == x.m)
            return c > x.c;
        return m > x.m;
    }
};
bool cmpx(line &a, line &b, line &c)
{
    return __int128_t(a.c - b.c) * (c.m - b.m) >= __int128_t(b.c - c.c) * (b.m - a.m);
}
bool cmpx(line &a, line &b, __int128_t c)
{
    int u = a.c - b.c, v = b.m - a.m;
    return (v > 0 ? u < c * v : u > c * v);
}
struct LineAddGetmin
{
    vector<line> lines;
    vector<line> tmp;
    vector<line> R;
    void Rebuild_CHT()
    {
        lines.clear();
        for (int i = 0; i < R.size(); i++)
        {
            // ignore parallel line with smaller c-component (it won't contribute to the max)
            if (i && R[i].m == R[i - 1].m)
                continue;
            line cur = R[i];
            while (lines.size() >= 2 && cmpx(cur, lines.back(), lines[lines.size() - 2]))
                lines.pop_back();
            lines.push_back(cur);
        }
        reverse(lines.begin(), lines.end());
    }
    void nb()
    {
        if (cur % B == 0)
        {
            // Build CHT
            int nr = R.size();
            sort(tmp.begin(), tmp.end());
            for (auto &i : tmp)
                R.push_back(i);
            tmp.clear();
            inplace_merge(R.begin(), R.begin() + nr, R.end());
            Rebuild_CHT();
        }
    }
    int _Query(int x)
    {
        if (!lines.size())
            return -3e18;
        auto cmp = [&](int idx, int x) { return cmpx(this->lines[idx], this->lines[idx + 1], x); };
        int idx = lower_bound(iota0.begin(), iota0.begin() + lines.size() - 1, x, cmp) - iota0.begin();
        return lines[idx].eval(x);
    }
    int cur = 0;
    void insert(line x)
    {
        x.m -= 2 * x.m;
        x.c -= 2 * x.c;
        tmp.push_back(x);
        cur++;
        nb();
    }
    int get_min(int X)
    {
        int MAX = _Query(X);
        for (auto &line : tmp)
            MAX = max(X * line.m + line.c, MAX);
        return -MAX;
    }
};
void Yoshi()
{
    int n, q;
    cin >> n >> q;
    LineAddGetmin lgm;
    for (int i = 0; i < n + q; i++)
    {
        int t = 0, a, b;
        if (i >= n)
            cin >> t;
        cin >> a;
        if (t == 0)
            cin >> b;
        if (t == 0)
            lgm.insert({(int32_t)a, b});
        else
            cout << lgm.get_min(a) << endl;
    }
}
signed main()
{
#ifndef yoshi_likes_e4
    ios::sync_with_stdio(0);
    cin.tie(0);
    if (fopen(problem ".inp", "r"))
    {
        freopen(problem ".inp", "r", stdin);
        freopen(problem ".out", "w", stdout);
    }
#endif
    init();
    int t = 1;
#if multitest
    cin >> t;
#endif
    while (t--)
        Yoshi();
}
