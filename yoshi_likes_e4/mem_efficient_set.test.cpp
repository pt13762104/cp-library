// @brief Memory efficient sqrt set
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
const int B = 4096;
template <int sz> struct Circular_Buffer_Deque
{
    int elems[sz];
    int start = 0, length = 0;
    void insert(int p, int x)
    {
        assert(size() < sz);
        int v[length - p];
        if (start + length <= sz)
            copy(elems + start + p, elems + start + length, v);
        else if (start + p < sz)
        {
            copy(elems + start + p, elems + sz, v);
            copy(elems, elems + start + length - sz, v + sz - start - p);
        }
        else
        {
            start -= sz;
            if (start + length <= sz)
                copy(elems + start + p, elems + start + length, v);
            else if (start + p < sz)
            {
                copy(elems + start + p, elems + sz, v);
                copy(elems, elems + start + length - sz, v + sz - start - p);
            }
            start += sz;
        }
        elems[(start + p) % sz] = x;
        start++;
        if (start + length <= sz)
            copy(v, v + length - p, elems + start + p);
        else if (start + p < sz)
        {
            copy(v, v + sz - start - p, elems + start + p);
            copy(v + sz - start - p, v + length - p, elems);
        }
        else
        {
            start -= sz;
            if (start + length <= sz)
                copy(v, v + length - p, elems + start + p);
            else if (start + p < sz)
            {
                copy(v, v + sz - start - p, elems + start + p);
                copy(v + sz - start - p, v + length - p, elems);
            }
            start += sz;
        }
        start--;
        length++;
    }
    void erase(int p)
    {
        assert(size());
        int v[length - p - 1];
        length--;
        start++;
        if (start + length <= sz)
            copy(elems + start + p, elems + start + length, v);
        else if (start + p < sz)
        {
            copy(elems + start + p, elems + sz, v);
            copy(elems, elems + start + length - sz, v + sz - start - p);
        }
        else
        {
            start -= sz;
            if (start + length <= sz)
                copy(elems + start + p, elems + start + length, v);
            else if (start + p < sz)
            {
                copy(elems + start + p, elems + sz, v);
                copy(elems, elems + start + length - sz, v + sz - start - p);
            }
            start += sz;
        }
        start--;
        if (start + length <= sz)
            copy(v, v + length - p, elems + start + p);
        else if (start + p < sz)
        {
            copy(v, v + sz - start - p, elems + start + p);
            copy(v + sz - start - p, v + length - p, elems);
        }
        else
        {
            start -= sz;
            if (start + length <= sz)
                copy(v, v + length - p, elems + start + p);
            else if (start + p < sz)
            {
                copy(v, v + sz - start - p, elems + start + p);
                copy(v + sz - start - p, v + length - p, elems);
            }
            start += sz;
        }
    }
    void push_back(int x)
    {
        assert(size() < sz);
        length++;
        elems[(start + length - 1) % sz] = x;
    }
    void push_front(int x)
    {
        assert(size() < sz);
        length++, start += sz - 1, start %= sz;
        elems[start] = x;
    }
    void pop_back()
    {
        assert(size());
        length--;
    }
    void pop_front()
    {
        assert(size());
        length--, start++, start %= sz;
    }
    int &operator[](int x)
    {
        return elems[(start + x) % sz];
    }
    int &front()
    {
        return elems[start];
    }
    int &back()
    {
        return elems[(start + length - 1) % sz];
    }
    int size()
    {
        return length;
    }
};
struct SQRT_set
{
    vector<Circular_Buffer_Deque<B + 1>> deques;
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