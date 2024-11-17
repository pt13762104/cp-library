// @brief Sqrt set
#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"
#pragma GCC optimize("O3,unroll-loops")
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
const int NMAX = 500000;
const int B = 5000;
template <int sz> struct fixed_buffer_deque
{
    int elems[2 * (sz + NMAX / B)];
    int start = sz + NMAX / B, end = sz + NMAX / B;
    void fix()
    {
        if (start == 0 || end == 2 * (sz + NMAX / B))
        {
            int cur_sz = end - start;
            move(elems + start, elems + end, elems + sz + NMAX / B);
            start = sz + NMAX / B;
            end = start + cur_sz;
        }
    }
    void insert(int p, int x)
    {
        assert(size() < sz);
        fix();
        if (end - p < p - start + 1)
        {
            move(elems + start + p, elems + end, elems + start + p + 1);
            elems[start + p] = x;
            end++;
        }
        else
        {
            move(elems + start, elems + start + p + 1, elems + start - 1);
            start--;
            elems[start + p] = x;
        }
    }
    void erase(int p)
    {
        assert(size());
        fix();
        if (end - p < p - start + 1)
        {
            move(elems + start + p + 1, elems + end, elems + start + p);
            end--;
        }
        else
        {
            move(elems + start, elems + start + p, elems + start + 1);
            start++;
        }
    }
    void push_back(int x)
    {
        assert(size() < sz);
        fix();
        elems[end] = x;
        end++;
    }
    void push_front(int x)
    {
        assert(size() < sz);
        fix();
        start--;
        elems[start] = x;
    }
    void pop_back()
    {
        assert(size());
        fix();
        end--;
    }
    void pop_front()
    {
        assert(size());
        fix();
        start++;
    }
    int &operator[](int x)
    {
        return elems[start + x];
    }
    int &front()
    {
        return elems[start];
    }
    int &back()
    {
        return elems[end - 1];
    }
    int size()
    {
        return end - start;
    }
};
struct SQRT_set
{
    vector<fixed_buffer_deque<B + 1>> deques;
    int n = 0;
    int get_kth(int x)
    {
        return deques[x / B][x % B];
    }
    int order(int x)
    {
        // equal to --upper_bound
        int l = -1;
        if (n)
            for (int R = 1 << __lg(n); R; R >>= 1)
            {
                if (l + R < n && get_kth(l + R) <= x)
                    l += R;
            }
        return l;
    }
    void insert(int x)
    {
        int pos = order(x);
        if (pos != -1 && get_kth(pos) == x)
            return;
        pos++;
        if (!deques.size() || deques.back().size() == B)
            deques.push_back({});
        deques[pos / B].insert(pos % B, x);
        if (deques[pos / B].size() > B)
        {
            int i = pos / B + 1;
            while (i < deques.size() && deques[i - 1].size() > B)
            {
                auto v = deques[i - 1].back();
                deques[i - 1].pop_back();
                deques[i].push_front(v);
                i++;
            }
        }
        if (!deques.back().size())
            deques.pop_back();
        n++;
    }
    void erase(int x)
    {
        int pos = order(x);
        if (pos == -1 || (pos != -1 && get_kth(pos) != x))
            return;
        deques[pos / B].erase(pos % B);
        int i = pos / B + 1;
        while (i < deques.size())
        {
            auto v = deques[i].front();
            deques[i].pop_front();
            deques[i - 1].push_back(v);
            i++;
        }
        if (!deques.back().size())
            deques.pop_back();
        n--;
    }
};
void Yoshi()
{
    SQRT_set s;
    int q;
    cin >> s.n >> q;
    for (int i = 0; i < s.n; i++)
    {
        int x;
        cin >> x;
        if (i % B == 0)
            s.deques.push_back({});
        s.deques.back().push_back(x);
    }
    while (q--)
    {
        int t, x;
        cin >> t >> x;
        if (t == 0)
        {
            s.insert(x);
        }
        else if (t == 1)
        {
            s.erase(x);
        }
        else if (t == 2)
        {
            if (x > s.n)
                cout << -1 << endl;
            else
                cout << s.get_kth(x - 1) << endl;
        }
        else if (t == 3)
        {
            cout << s.order(x) + 1 << endl;
        }
        else if (t == 4)
        {
            int v = s.order(x);
            if (v == -1)
                cout << -1 << endl;
            else
                cout << s.get_kth(v) << endl;
        }
        else
        {
            int v = s.order(x);
            if (v == -1 || s.get_kth(v) != x)
                v++;
            if (v == s.n)
                cout << -1 << endl;
            else
                cout << s.get_kth(v) << endl;
        }
    }
}
signed main()
{
#ifndef yoshi_likes_e4
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
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
