// @brief Multiplication of Big Integers
#define PROBLEM "https://judge.yosupo.jp/problem/multiplication_of_big_integers"
#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define problem ""
#define multitest 1
#define debug(x) cerr << #x << " = " << x << endl;
const int mod1 = 998244353;
const int mod2 = 943718401;
const int n1 = 145187429;
const int n2 = 844668317;
uint64_t primitive_root_cache_1st[1 << 19];
uint64_t primitive_root_cache_2nd[1 << 19];
chrono::high_resolution_clock Clock;
void init()
{
    primitive_root_cache_1st[0] = 1;
    for (int i = 1; i < 1 << 19; i++)
        primitive_root_cache_1st[i] = (int64_t)primitive_root_cache_1st[i - 1] * 275981743 % mod1;
    primitive_root_cache_2nd[0] = 1;
    for (int i = 1; i < 1 << 19; i++)
        primitive_root_cache_2nd[i] = (int64_t)primitive_root_cache_2nd[i - 1] * 135213552 % mod2;
}
int inv(long long n, long long mod)
{
    long long k = mod - 2, res = 1;
    while (k)
    {
        if (k & 1)
        {
            res *= n;
            res %= mod;
        }
        n *= n;
        n %= mod;
        k >>= 1;
    }
    return res;
}
void dft_1st(vector<unsigned int> &A)
{
    auto t1 = Clock.now();
    for (int i = 1, j = 0; i < A.size(); ++i)
    {
        int bit = A.size() / 2;
        for (; j >= bit; bit /= 2)
            j -= bit;
        j += bit;
        if (i < j)
            swap(A[i], A[j]);
    }
    if (A.size() > 1000000)
        cerr << "Bit reversal: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms"
             << endl;
    t1 = Clock.now();
    for (int s = 1; s <= __lg(A.size()); s++)
    {
        int m = 1 << s;
        for (int k = 0; k < A.size(); k += m)
            for (int j = 0; j < (m >> 1); j++)
            {
                int t = (primitive_root_cache_1st[((1 << 20) / m) * j] * A[k + j + (m >> 1)]) % mod1, u = A[k + j];
                A[k + j] = (u + t) - mod1 * (u + t >= mod1);
                A[k + j + (m >> 1)] = (u + mod1 - t) - mod1 * (u >= t);
            }
    }
    if (A.size() > 1000000)
        cerr << "FFT: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
}
void idft_1st(vector<unsigned int> &a)
{
    int invn = inv(a.size(), mod1);
    reverse(a.begin() + 1, a.end());
    dft_1st(a);
    for (auto &i : a)
        i = (uint64_t)i * invn % mod1;
}
void dft_2nd(vector<unsigned int> &A)
{
    auto t1 = Clock.now();
    for (int i = 1, j = 0; i < A.size(); ++i)
    {
        int bit = A.size() / 2;
        for (; j >= bit; bit /= 2)
            j -= bit;
        j += bit;
        if (i < j)
            swap(A[i], A[j]);
    }
    if (A.size() > 1000000)
        cerr << "Bit reversal: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms"
             << endl;
    t1 = Clock.now();
    for (int s = 1; s <= __lg(A.size()); s++)
    {
        int m = 1 << s;
        for (int k = 0; k < A.size(); k += m)
            for (int j = 0; j < (m >> 1); j++)
            {
                int t = (primitive_root_cache_2nd[((1 << 20) / m) * j] * A[k + j + (m >> 1)]) % mod2, u = A[k + j];
                A[k + j] = (u + t) - mod2 * (u + t >= mod2);
                A[k + j + (m >> 1)] = (u + mod2 - t) - mod2 * (u >= t);
            }
    }
    if (A.size() > 1000000)
        cerr << "FFT: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
}
void idft_2nd(vector<unsigned int> &a)
{
    int invn = inv(a.size(), mod2);
    reverse(a.begin() + 1, a.end());
    dft_2nd(a);
    for (auto &i : a)
        i = (uint64_t)i * invn % mod2;
}
int pow10[6] = {1, 10, 100, 1000, 10000, 100000};
void Yoshi()
{
    string a, b;
    cin >> a >> b;
    vector<unsigned int> A6(a.size() / 6 + 1), B6(b.size() / 6 + 1);
    int sign = 1, i = 0;
    while (a.size() && a.back() >= '0' && a.back() <= '9')
    {
        A6[i / 6] += (a.back() - 48) * (pow10[i % 6]);
        a.pop_back();
        i++;
    }
    i = 0;
    while (b.size() && b.back() >= '0' && b.back() <= '9')
    {
        B6[i / 6] += (b.back() - 48) * (pow10[i % 6]);
        b.pop_back();
        i++;
    }
    if (A6 == vector<unsigned int>(1) || B6 == vector<unsigned int>(1))
    {
        cout << 0 << endl;
        return;
    }
    if (a.size())
        sign *= -1;
    if (b.size())
        sign *= -1;
    int sz = 1 << __lg(A6.size() + B6.size());
    if (sz < A6.size() + B6.size())
        sz <<= 1;
    A6.resize(sz);
    B6.resize(sz);
    vector<unsigned int> aa_1 = A6, bb_1 = B6, aa_2 = A6, bb_2 = B6;
    dft_1st(aa_1);
    dft_1st(bb_1);
    for (int i = 0; i < sz; i++)
        aa_1[i] = ((int64_t)aa_1[i] * bb_1[i]) % mod1;
    idft_1st(aa_1);
    dft_2nd(aa_2);
    dft_2nd(bb_2);
    for (int i = 0; i < sz; i++)
        aa_2[i] = ((int64_t)aa_2[i] * bb_2[i]) % mod2;
    idft_2nd(aa_2);
    vector<long long> cc_raw(sz);
    for (int i = 0; i < sz; i++)
        cc_raw[i] = ((__int128_t)aa_1[i] * mod2 * n2 + (__int128_t)aa_2[i] * mod1 * n1) % ((__int64_t)mod1 * mod2);
    for (int i = 0; i < cc_raw.size() - 1; i++)
    {
        cc_raw[i + 1] += cc_raw[i] / 1000000;
        cc_raw[i] %= 1000000;
    }
    vector<int> cc(sz * 6);
    for (int i = 0; i < cc_raw.size(); i++)
    {
        cc[i * 6] += cc_raw[i] % 10;
        cc[i * 6 + 1] += cc_raw[i] / 10 % 10;
        cc[i * 6 + 2] += cc_raw[i] / 100 % 10;
        cc[i * 6 + 3] += cc_raw[i] / 1000 % 10;
        cc[i * 6 + 4] += cc_raw[i] / 10000 % 10;
        cc[i * 6 + 5] += cc_raw[i] / 100000 % 10;
    }
    for (int i = 0; i < cc.size() - 1; i++)
    {
        cc[i + 1] += cc[i] / 10;
        cc[i] %= 10;
    }
    string s = "";
    if (sign != 1)
        s += '-';
    while (!cc.back())
        cc.pop_back();
    while (cc.size())
    {
        s += cc.back() + '0';
        cc.pop_back();
    }
    cout << s << endl;
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
