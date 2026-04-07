// @brief Segment Add Get Min (CDQ)
#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"
#include <bits/stdc++.h>
using namespace std;
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
void init()
{
}
struct line
{
    int m;
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
struct CHT
{
    bool cmpx(line &a, line &b, line &c)
    {
        return __int128_t(a.c - b.c) * (c.m - b.m) >= __int128_t(b.c - c.c) * (b.m - a.m);
    }
    bool cmpx(line &a, line &b, int c)
    {
        long long u = a.c - b.c, v = b.m - a.m;
        return (v > 0 ? u < v * c : u > v * c);
    }
    vector<line> lines;
    CHT()
    {
    }
    int idx = 0;
    void addLine(line cur)
    {
        // ignore parallel line with smaller c-component (it won't contribute to the max)
        if (lines.size() && cur.m == lines.back().m)
            return;
        while (lines.size() >= 2 && cmpx(cur, lines.back(), lines[lines.size() - 2]))
            lines.pop_back();
        lines.push_back(cur);
        idx = lines.size() - 1;
    }
    long long _Query(int x)
    {
        if (!lines.size())
            return -3e18;
        while (idx > 0 && cmpx(this->lines[idx - 1], this->lines[idx], x))
            idx--;
        return lines[idx].eval(x);
    }
};
struct Qu
{
    // l is mapped to p when r is < -1e9
    int l, r, a;
    long long b;
};
vector<long long> qres;
vector<Qu> qus;
vector<int> lso, rso, aso, tmpu_id;
struct Segment
{
    int l, r;
    bool operator<(const Segment &c) const
    {
        return r < c.r;
    }
};
struct ST
{
    vector<CHT> t;
    int n = 0;
    ST()
    {
    }
    ST(int n) : n(n)
    {
        t.resize(2 * n);
    }
    long long query(int p, int x)
    {
        long long mn = -3e18;
        p += n;
        while (p)
        {
            mn = max(mn, t[p]._Query(x));
            p >>= 1;
        }
        return -mn;
    }
    void modify(int l, int r, int a, long long b)
    {
        for (l += n, r += n; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
                t[l++].addLine({a, b});
            if (r & 1)
                t[--r].addLine({a, b});
        }
    }
};
void DNC(int l, int r)
{
    if (r - l > 200)
    {
        int mid = (l + r) >> 1;
        DNC(l, mid);
        DNC(mid + 1, r);
        vector<int> lsA(lso.begin() + l, lso.begin() + mid + 1);
        vector<int> lsB(lso.begin() + mid + 1, lso.begin() + r + 1);
        vector<int> rsA(rso.begin() + l, rso.begin() + mid + 1);
        vector<int> rsB(rso.begin() + mid + 1, rso.begin() + r + 1);
        vector<int> asA(aso.begin() + l, aso.begin() + mid + 1);
        vector<int> asB(aso.begin() + mid + 1, aso.begin() + r + 1);
        vector<Qu> tmpu;
        vector<pair<int, int>> tmpq;
        for (auto &m : lsB)
            if (qus[m].r < -1e9)
                tmpq.push_back({qus[m].l, m});
        if (tmpq.size())
        {
            for (auto &m : lsA)
                if (qus[m].r >= -1e9)
                {
                    tmpu_id[m] = tmpu.size();
                    tmpu.push_back(qus[m]);
                }
            int pos = 0;
            for (auto &[l, r, a, b] : tmpu)
            {
                while (pos < tmpq.size() && tmpq[pos].first < l)
                    pos++;
                l = pos;
            }
            pos = 0;
            for (auto &m : rsA)
                if (qus[m].r >= -1e9)
                {
                    while (pos < tmpq.size() && tmpq[pos].first <= qus[m].r)
                        pos++;
                    tmpu[tmpu_id[m]].r = pos;
                }
            ST t(tmpq.size());
            for (auto &m : asA)
                if (qus[m].r >= -1e9)
                {
                    auto [l, r, a, b] = tmpu[tmpu_id[m]];
                    t.modify(l, r, -a, -b);
                }
            int qu_id = 0;
            for (auto &[p, id] : tmpq)
                qres[id] = min(qres[id], t.query(qu_id++, p));
        }
        merge(lsA.begin(), lsA.end(), lsB.begin(), lsB.end(), lso.begin() + l,
              [&](auto &a, auto &b) { return qus[a].l < qus[b].l; });
        merge(rsA.begin(), rsA.end(), rsB.begin(), rsB.end(), rso.begin() + l,
              [&](auto &a, auto &b) { return qus[a].r < qus[b].r; });
        merge(asA.begin(), asA.end(), asB.begin(), asB.end(), aso.begin() + l, [&](auto &a, auto &b) {
            if (qus[a].a == qus[b].a)
                return qus[a].b < qus[b].b;
            return qus[a].a < qus[b].a;
        });
    }
    else
    {
        vector<int> idx;
        for (int i = l; i <= r; i++)
            if (qus[i].r >= -1e9)
                idx.push_back(i);
        for (int i = l; i <= r; i++)
            if (qus[i].r < -1e9)
                for (auto &j : idx)
                {
                    if (j > i)
                        break;
                    if (qus[j].l <= qus[i].l && qus[i].l <= qus[j].r)
                        qres[i] = min(qres[i], (long long)(qus[j].a) * qus[i].l + qus[j].b);
                }
        sort(lso.begin() + l, lso.begin() + r + 1, [&](auto &a, auto &b) { return qus[a].l < qus[b].l; });
        sort(rso.begin() + l, rso.begin() + r + 1, [&](auto &a, auto &b) { return qus[a].r < qus[b].r; });
        sort(aso.begin() + l, aso.begin() + r + 1, [&](auto &a, auto &b) {
            if (qus[a].a == qus[b].a)
                return qus[a].b < qus[b].b;
            return qus[a].a < qus[b].a;
        });
    }
}
void Yoshi()
{
    int n, q;
    cin >> n >> q;
    qus.resize(n + q);
    tmpu_id.resize(n + q);
    qres.assign(n + q, 3e18);
    lso.resize(n + q);
    iota(lso.begin(), lso.end(), 0);
    rso = lso, aso = rso;
    for (int i = 0; i < n + q; i++)
    {
        int a = 0;
        if (i >= n)
            cin >> a;
        if (a == 0)
            cin >> qus[i].l >> qus[i].r >> qus[i].a >> qus[i].b, qus[i].r--;
        else
            cin >> qus[i].l, qus[i].r = -1e9 - 1;
    }
    DNC(0, n + q - 1);
    for (int i = 0; i < n + q; i++)
        if (qus[i].r < -1e9)
            cout << (qres[i] > 2e18 ? "INFINITY" : to_string(qres[i])) << endl;
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