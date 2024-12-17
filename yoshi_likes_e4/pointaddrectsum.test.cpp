#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
const int FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
void init()
{
}
struct Query
{
    int t, l, r, d, u, x, y, w;
};
long long qres[200000];
struct Fenwick
{
    vector<long long> bit;
    Fenwick(int n) : bit(n + 1, 0)
    {
    }
    void update(int id, int val)
    {
        for (; id < bit.size(); id += id & (-id))
            bit[id] += val;
    }
    long long query(int id)
    {
        long long sum = 0;
        for (; id > 0; id -= id & (-id))
            sum += bit[id];
        return sum;
    }
};
Fenwick BIT(200000);
struct Event
{
    int a, b, id, type;
};
vector<Event> events;
vector<Query> queries;
void DNC(int l, int r)
{
    if (r - l > 16)
    {
        int mid = (l + r) >> 1;
        // calculate the contribution
        vector<Event> updl, qryr;
        for (int i = l; i <= mid; i++)
            if (events[i].id == -1)
                updl.push_back(events[i]);
        for (int i = mid + 1; i <= r; i++)
            if (events[i].id != -1)
                qryr.push_back(events[i]);
        sort(updl.begin(), updl.end(), [](Event &a, Event &b) { return a.a < b.a; });
        sort(qryr.begin(), qryr.end(), [](Event &a, Event &b) { return a.a < b.a; });
        int uid = 0;
        for (int i = 0; i < qryr.size(); i++)
        {
            while (uid < updl.size() && updl[uid].a <= qryr[i].a)
            {
                BIT.update(updl[uid].b, updl[uid].type);
                uid++;
            }
            qres[qryr[i].id] += qryr[i].type * BIT.query(qryr[i].b);
        }
        for (int i = 0; i < uid; i++)
            BIT.update(updl[i].b, -updl[i].type);
        DNC(l, mid);
        DNC(mid + 1, r);
    }
    else if (l < r)
    {
        for (int i = l; i <= r; i++)
            if (events[i].id != -1)
            {
                long long r = 0;
                for (int j = l; j < i; j++)
                    if (events[j].a <= events[i].a && events[j].b <= events[i].b && events[j].id == -1)
                        r += events[j].type;
                qres[events[i].id] += events[i].type * r;
            }
    }
}
void Yoshi()
{
    int n, q;
    cin >> n >> q;
    vector<int> xv, yv;
    for (int i = 0; i < n; i++)
    {
        int x, y, w;
        cin >> x >> y >> w;
        queries.push_back({0, 0, 0, 0, 0, x, y, w});
        xv.push_back(x);
        yv.push_back(y);
    }
    for (int i = 0; i < q; i++)
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
    for (int i = 0; i < n + q; i++)
    {
        auto &qu = queries[i];
        if (qu.t)
        {
            int id1 = lower_bound(xv.begin(), itx, qu.l) - xv.begin();
            int id2 = lower_bound(xv.begin(), itx, qu.r) - xv.begin();
            int id3 = lower_bound(yv.begin(), ity, qu.d) - yv.begin();
            int id4 = lower_bound(yv.begin(), ity, qu.u) - yv.begin();
            events.push_back({id2, id4, i, 1});
            events.push_back({id1, id3, i, 1});
            events.push_back({id1, id4, i, -1});
            events.push_back({id2, id3, i, -1});
        }
        else
        {
            int id5 = upper_bound(xv.begin(), itx, qu.x) - xv.begin();
            int id6 = upper_bound(yv.begin(), ity, qu.y) - yv.begin();
            events.push_back({id5, id6, -1, qu.w});
        }
    }
    chrono::high_resolution_clock Clock;
    auto t1 = Clock.now();
    DNC(0, events.size() - 1);
    cerr << "DNC time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << "ms" << endl;
    for (int i = 0; i < n + q; i++)
        if (queries[i].t)
            cout << qres[i] << endl;
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
