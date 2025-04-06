// @brief Determinant of Sparse Matrix
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
using namespace std;
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define PROBLEM "https://judge.yosupo.jp/problem/sparse_matrix_det"
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
struct chash
{
    uint64_t operator()(uint64_t x) const noexcept
    {
        // splitmix64
        uint64_t z = (x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
};
const int MOD = 998244353;
#define int long long
int bpow(int n, int k)
{
    int res = 1;
    while (k)
    {
        if (k & 1)
        {
            res *= n;
            res %= MOD;
        }
        n *= n;
        n %= MOD;
        k >>= 1;
    }
    return res;
}
int inv(int i)
{
    return bpow(i, MOD - 2);
}
int us(int a, int b)
{
    int r = (a - b) % MOD;
    return r + (r < 0 ? MOD : 0);
}
#undef int
const int B = 64;
struct PredProblem
{
    int n, h, sz;
    vector<vector<uint64_t>> tree;
    void init(int old_n)
    {
        n = old_n;
        while (n > 1)
        {
            // n = ceil(n/B)
            tree.push_back(vector<uint64_t>(n = (n + B - 1) / B));
        }
        n = old_n, h = tree.size();
    };
    // remember: tree[i][k][j] means that at the jth subtree of tree[i][k] has sum > 0
    // This is copied from noimi
    void insert(int i)
    {
        if (get(i))
            return;
        sz++;
        // log_B(n)
        for (auto &v : tree)
        {
            v[i / B] |= 1ULL << (i % B);
            i /= B;
        }
    };
    void erase(int i)
    {
        if (!get(i))
            return;
        sz--;
        // log_B(n)
        for (auto &v : tree)
        {
            v[i / B] &= ~(1ULL << (i % B));
            i /= B;
            // do not erase the above subtrees if v[i] is not empty
            if (v[i])
                break;
        }
    };
    int size() { return sz; }
    bool get(int i)
    {
        return (tree[0][i / B] >> (i % B)) & 1;
    };
    int next(int i)
    {
        // log_B(n)
        for (int j = 0; j < h; j++)
        {
            // get the position in the current subtree
            if (i / B >= tree[j].size())
                break;
            int pos = i % B;
            uint64_t c = tree[j][i / B];
            // mask off the first pos bits
            c &= ~((1ULL << pos) - 1);
            i /= B;
            if (!c)
            {
                // jump to the next tree
                i++;
            }
            else
            {
                // if we find the first bit set, try to go deeper
                int i2 = i * B + __builtin_ctzll(c);
                for (int k = j - 1; k >= 0; k--)
                {
                    i2 *= B;
                    i2 += __builtin_ctzll(tree[k][i2 / B]);
                }
                return i2;
            }
        }
        return n;
    };
    int prev(int i)
    {
        // log_B(n)
        for (int j = 0; j < h; j++)
        {
            // get the position in the current subtree
            int pos = i % B;
            uint64_t c = tree[j][i / B];
            // keep only the first pos bits
            if (pos != B - 1)
                c %= (1ULL << (pos + 1));
            i /= B;
            if (!c)
            {
                // jump to the PREVIOUS tree
                if (!i)
                    break;
                i--;
            }
            else
            {
                // if we find the last bit set, try to go deeper
                int i2 = i * B + 63 - __builtin_clzll(c);
                for (int k = j - 1; k >= 0; k--)
                {
                    i2 *= B;
                    i2 += 63 - __builtin_clzll(tree[k][i2 / B]);
                }
                return i2;
            }
        }
        return -1;
    };
};
int i_stack[3000];
int64_t __ref(vector<vector<int>> &x)
{
    vector<PredProblem *> Nz(x.size());
    for (int i = 0; i < x.size(); i++)
    {
        Nz[i] = new PredProblem();
        Nz[i]->init(x.size() + 5);
    }
    for (int i = 0; i < x.size(); i++)
        for (int idx = 0; idx < x.size(); idx++)
            Nz[i]->insert(idx);
    int h = 0, k = 0;
    int64_t sign = 1;
    while (h < x.size() && k < x.size())
    {
        int cur_last = 1e9, idx = x.size();
        for (int i = h; i < x.size(); i++)
        {
            if (x[i][k])
                if (Nz[i]->size() < cur_last)
                {
                    cur_last = Nz[i]->size();
                    idx = i;
                }
        }
        if (idx != x.size())
        {
            swap(x[h], x[idx]);
            swap(Nz[h], Nz[idx]);
            const int64_t ixhk = inv(x[h][k]);
            if (idx != h)
            {
                sign *= (MOD - 1);
                sign %= MOD;
            }
            int i_ptr = 0;
            int j = Nz[h]->next(0);
            while (j < x.size())
            {
                i_stack[i_ptr++] = j;
                j = Nz[h]->next(j + 1);
            }
            for (int i = h + 1; i < x.size(); i++)
            {
                int64_t coeff = (ixhk * x[i][k]) % MOD;
                for (int k = 0; k < i_ptr; k++)
                {
                    auto j = i_stack[k];
                    x[i][j] = us(x[i][j], coeff * x[h][j]);
                    if (!x[i][j])
                        Nz[i]->erase(j);
                    if (x[i][j])
                        Nz[i]->insert(j);
                }
            }
            h++;
        }
        k++;
    }
    return sign;
}
int rd()
{
    int k = 0, f = 1;
    char s = getchar();
    while (s < '0' || s > '9')
    {
        if (s == '-')
            f = 0;
        s = getchar();
    }
    while (s >= '0' && s <= '9')
    {
        k = (k << 1) + (k << 3) + (s ^ '0');
        s = getchar();
    }
    return f ? k : -k;
}
void init()
{
}
void Yoshi()
{
    int N = rd(), K = rd();
    vector<vector<int>> raw(N, vector<int>(N));
    while (K--)
    {
        int a = rd(), b = rd(), x = rd();
        raw[a][b] = x;
    }
    vector<pair<int, int>> st;
    for (int i = 0; i < raw.size(); i++)
    {
        int non_zero = 0;
        for (auto &k : raw[i])
            non_zero += (bool)k;
        st.push_back({non_zero, i});
    }
    int64_t sgn = 1;
    for (int i = 0; i < raw.size(); i++)
    {
        int ptr = min_element(st.begin() + i, st.end()) - st.begin();
        if (ptr != i)
        {
            sgn *= MOD - 1, sgn %= MOD;
            swap(raw[st[i].second], raw[st[ptr].second]);
            swap(st[i], st[ptr]);
        }
    }
    // gaussian elimination
    int64_t sign = __ref(raw);
    int64_t determinant = (sign * sgn) % MOD;
    for (int i = 0; i < N; i++)
    {
        determinant *= raw[i][i];
        determinant %= MOD;
    }
    cout << determinant << endl;
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
