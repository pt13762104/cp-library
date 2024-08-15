// @brief Range Affine Range Sum
#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"
#include <iostream>
using namespace std;
#define int long long
const int N = 524288;
int t[2 * N], add[N], mult[N];
bool to_update[N];
int n, h, MOD = 998244353;
void fastmod(long long &i)
{
    i -= 998244353 * (long long)((i * __uint128_t(4960468989641778720)) >> 92);
}
void apply(int p, int b, int c, int k)
{
    t[p] *= b;
    t[p] += c * k;
    fastmod(t[p]);
    if (p < n)
    {
        to_update[p] = 1;
        mult[p] *= b;
        fastmod(mult[p]);
        add[p] *= b;
        add[p] += c;
        fastmod(add[p]);
    }
}
void calc(int p, int k)
{
    if (to_update[p])
    {
        apply(p << 1, mult[p], add[p], k >> 1);
        apply(p << 1 | 1, mult[p], add[p], k >> 1);
        to_update[p] = 0;
        mult[p] = 1;
        add[p] = 0;
    }
    t[p] = (t[p << 1] + t[p << 1 | 1]);
    fastmod(t[p]);
}
void build(int l, int r)
{
    int k = 2;
    for (l += n, r += n - 1; l > 1; k <<= 1)
    {
        l >>= 1, r >>= 1;
        for (int i = r; i >= l; --i)
            calc(i, k);
    }
}

void push(int l, int r)
{
    int s = h, k = 1 << (h - 1);
    for (l += n, r += n - 1; s > 0; --s, k >>= 1)
        for (int i = l >> s; i <= r >> s; ++i)
            if (to_update[i])
            {
                apply(i << 1, mult[i], add[i], k);
                apply(i << 1 | 1, mult[i], add[i], k);
                to_update[i] = 0;
                mult[i] = 1;
                add[i] = 0;
            }
}
void modify(int l, int r, int b, int c)
{
    push(l, l + 1);
    push(r - 1, r);
    int l0 = l, r0 = r, k = 1;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1, k <<= 1)
    {
        if (l & 1)
            apply(l++, b, c, k);
        if (r & 1)
            apply(--r, b, c, k);
    }
    build(l0, l0 + 1);
    build(r0 - 1, r0);
}

int query(int l, int r)
{
    push(l, l + 1);
    push(r - 1, r);
    int res = 0;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1)
    {
        if (l & 1)
        {
            res += t[l++];
        }
        if (r & 1)
        {
            res += t[--r];
        }
    }
    fastmod(res);
    return res;
}
signed main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int q;
    cin >> n >> q;
    int nn = n;
    n = N;
    h = sizeof(int) * 8 - __builtin_clzll(n);
    for (int i = 0; i < nn; i++)
    {
        cin >> t[n + i];
    }
    for (int i = 0; i < n; i++)
        mult[i] = 1;
    build(0, n);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            int b, c;
            cin >> b >> c;
            modify(l, r, b, c);
        }
        else
        {
            cout << query(l, r) << '\n';
        }
    }
}