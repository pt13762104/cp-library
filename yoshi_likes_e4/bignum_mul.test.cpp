#define PROBLEM "https://judge.yosupo.jp/problem/multiplication_of_big_integers"
#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;
const double PI = acos(-1);
typedef double f64x4 __attribute__((vector_size(32)));
typedef double f64x2 __attribute__((vector_size(16)));
inline f64x4 mul(f64x4 a, f64x4 b)
{
    return f64x4{a[0] * b[0] - a[1] * b[1], a[0] * b[1] + a[1] * b[0], a[2] * b[2] - a[3] * b[3],
                 a[2] * b[3] + a[3] * b[2]};
}
inline f64x4 mul(f64x2 a, f64x4 b)
{
    double r = a[0], i = a[1];
    return r * b - f64x4{i * b[1], -i * b[0], i * b[3], -i * b[2]};
}
inline f64x2 mul(f64x2 a, f64x2 b)
{
    return f64x2{a[0] * b[0] - a[1] * b[1], a[0] * b[1] + a[1] * b[0]};
}
void dft(f64x2 *A, int n, bool invert = 0)
{
    int lga = __lg(n);
    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n / 2;
        for (; j >= bit; bit /= 2)
            j -= bit;
        j += bit;
        if (i < j)
            swap(A[i], A[j]);
    }
    double mf = (invert ? 0.5 : 1);
    alignas(32) f64x4 *AA = (f64x4 *)A;
    for (int k = 0; k < n; k += 2)
    {
        f64x2 t = A[k + 1], u = A[k];
        A[k] = (u + t) * mf;
        A[k + 1] = (u - t) * mf;
    }
    for (int s = 2; s <= lga; s++)
    {
        int m = 1 << (s - 1);
        f64x4 ww{1, 0, cos(2 * (invert ? -1 : 1) * PI / (1 << s)), sin(2 * (invert ? -1 : 1) * PI / (1 << s))};
        f64x2 tw{cos(2 * (invert ? -1 : 1) * PI / m), sin(2 * (invert ? -1 : 1) * PI / m)};
        for (int k = 0; k < n >> 1; k += m)
        {
            f64x4 w1 = ww;
            for (int j = 0; j < (m >> 1); j++, w1 = mul(tw, w1))
            {
                f64x4 u = AA[k + j], t = mul(w1, AA[k + j + (m >> 1)]);
                AA[k + j] = (u + t) * mf;
                AA[k + j + (m >> 1)] = (u - t) * mf;
            }
        }
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin >> t;
    while (t--)
    {
        string a, b;
        cin >> a >> b;
        int sz = a.size() + b.size() - (a[0] == '-') - (b[0] == '-'), old_sz = sz;
        sz = 1 << __lg(sz);
        if (sz < old_sz)
            sz *= 2;
        alignas(32) f64x2 aa[sz], bb[sz], cc[sz];
        int c[sz];
        memset(aa, 0, sizeof(f64x2) * sz);
        memset(bb, 0, sizeof(f64x2) * sz);
        memset(cc, 0, sizeof(f64x2) * sz);
        memset(c, 0, sizeof(int) * sz);
        int sign = (a[0] == '-' ? -1 : 1) * (b[0] == '-' ? -1 : 1);
        for (int i = a.size() - 1, j = 0; i >= (a[0] == '-'); i--, j++)
            aa[j] = f64x2{a[i] - 48, 0};
        for (int i = b.size() - 1, j = 0; i >= (b[0] == '-'); i--, j++)
            bb[j] = f64x2{b[i] - 48, 0};
        if (sz > 64)
        {
            dft(aa, sz);
            dft(bb, sz);
            for (int i = 0; i < sz; i++)
                cc[i] = mul(aa[i], bb[i]);
            dft(cc, sz, 1);
        }
        else
        {
            for (int i = 0; i < a.size() - (a[0] == '-'); i++)
                for (int j = 0; j < b.size() - (b[0] == '-'); j++)
                    cc[i + j] += aa[i] * bb[j];
        }
        for (int i = 0; i < sz - 1; i++)
        {
            c[i] += cc[i][0] + 0.5;
            c[i + 1] += c[i] / 10;
            c[i] %= 10;
        }
        int ptr = old_sz - 1;
        while (ptr >= 0 && !c[ptr])
            ptr--;
        if (ptr == -1)
            cout << 0;
        else
        {
            if (sign != 1)
                cout << '-';
            for (int i = ptr; i >= 0; i--)
                cout << c[i];
        }
        cout << '\n';
    }
}