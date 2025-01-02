// @brief Range Linear Add Range Min
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define int long long
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
const size_t FIXED_RANDOM = chrono::high_resolution_clock::now().time_since_epoch().count();
void init()
{
}
int fdiv(int a, int32_t b)
{ // floored division
    return a / b - ((a ^ b) < 0 && a % b);
}
struct line
{
    int32_t m;
    long long c;
    long long eval(long long x)
    {
        return m * x + c;
    }
    long long intersectX(line l)
    {
        return fdiv(c - l.c, l.m - m);
    }
    bool operator<(const line &x) const
    {
        if (m == x.m)
            return c > x.c;
        return m > x.m;
    }
};
struct Static_CHT
{
    vector<line> lines;
    vector<int> isects;
    Static_CHT()
    {
    }
    Static_CHT(vector<line> p)
    {
        for (int i = 0; i < p.size(); i++)
        {
            line cur = p[i];
            while (lines.size() >= 2 &&
                   cur.intersectX(lines.back()) >= lines.back().intersectX(lines[lines.size() - 2]))
                lines.pop_back();
            lines.push_back(cur);
        }
        reverse(lines.begin(), lines.end());
        isects.resize(max(0ll, (int)lines.size() - 1));
        for (int i = 0; i < isects.size(); i++)
            isects[i] = this->lines[i].intersectX(this->lines[i + 1]);
    }
    int Query(int x)
    {
        if (!lines.size())
            return -3e18;
        int idx = lower_bound(isects.begin(), isects.end(), x) - isects.begin();
        return lines[idx].eval(x);
    }
};
const int B = 125;
vector<line> R[800];
Static_CHT C[800];
int lz_b[800], lz_c[800];
void Yoshi()
{
    int n, q;
    cin >> n >> q;
    for (int32_t i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        R[i / B].push_back({-i, -x});
    }
    for (int i = 0; i < n; i += B)
        C[i / B] = Static_CHT(R[i / B]);
    while (q--)
    {
        int T;
        cin >> T;
        if (T)
        {
            int l, r;
            cin >> l >> r;
            r--;
            int MAX = -3e18;
            if (l / B == r / B)
            {
                for (int i = l; i <= r; i++)
                    MAX = max(MAX, R[i / B][i % B].eval(lz_b[i / B]) + lz_c[i / B]);
            }
            else
            {
                for (int i = l; i < (l / B + 1) * B; i++)
                    MAX = max(MAX, R[i / B][i % B].eval(lz_b[i / B]) + lz_c[i / B]);
                for (int i = (r / B) * B; i <= r; i++)
                    MAX = max(MAX, R[i / B][i % B].eval(lz_b[i / B]) + lz_c[i / B]);
                for (int i = l / B + 1; i < r / B; i++)
                    MAX = max(MAX, C[i].Query(lz_b[i]) + lz_c[i]);
            }
            cout << -MAX << endl;
        }
        else
        {
            int l, r, b, c;
            cin >> l >> r >> b >> c;
            r--;
            if (l / B == r / B)
            {
                for (int i = l; i <= r; i++)
                    R[i / B][i % B].c -= b * i + c;
                C[l / B] = Static_CHT(R[l / B]);
            }
            else
            {
                for (int i = l; i < (l / B + 1) * B; i++)
                    R[i / B][i % B].c -= b * i + c;
                for (int i = (r / B) * B; i <= r; i++)
                    R[i / B][i % B].c -= b * i + c;
                for (int i = l / B + 1; i < r / B; i++)
                    lz_b[i] += b, lz_c[i] -= c;
                C[l / B] = Static_CHT(R[l / B]);
                C[r / B] = Static_CHT(R[r / B]);
            }
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
