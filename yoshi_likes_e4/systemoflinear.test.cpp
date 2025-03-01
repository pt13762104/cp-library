#include <bits/stdc++.h>
#pragma GCC optimize("Ofast,unroll-loops")
using namespace std;
#define int long long
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
const int MOD = 998244353;
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
chrono::high_resolution_clock Clock;
vector<vector<int>> RREF(vector<vector<int>> x, int N, int M)
{
    auto t1 = Clock.now();
    int h = 0, k = 0;
    while (h < N && k < M)
    {
        // Any non-zero pivot row works
        int idx = h;
        while (idx < x.size() && x[idx][k] == 0)
            idx++;
        if (idx != x.size())
        {
            swap(x[h], x[idx]);
            for (int i = h + 1; i < x.size(); i++)
            {
                int coeff = (x[i][k] * inv(x[h][k])) % MOD;
                for (int j = k; j < x[0].size(); j++)
                    x[i][j] = us(x[i][j], coeff * x[h][j]);
            }
            h++;
        }
        k++;
    }
    cerr << "rref time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
    return x;
}
#define getchar getchar_unlocked
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
bool rd_bool()
{
    char c = getchar();
    while ((c != '0') && (c != '1'))
        c = getchar();
    return c ^ '0';
}
void init()
{
}
void Yoshi()
{
    int N, M;
    N = rd(), M = rd();
    auto t0 = Clock.now();
    vector<vector<int>> raw(N, vector<int>(M + 1));
    string s;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            raw[i][j] = rd();
    for (int i = 0; i < N; i++)
        raw[i][M] = rd();
    cerr << "input time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t0).count() << " ms" << endl;
    auto rref = RREF(raw, N, M);
    for (int i = 0; i < N; i++)
        if (count(rref[i].begin(), rref[i].end(), 0) == M && rref[i][M])
        {
            cout << -1 << endl;
            return;
        }
    vector<vector<int>> pruned_rref;
    for (int i = 0; i < N; i++)
        if (count(rref[i].begin(), rref[i].end(), 0) != M + 1)
            pruned_rref.push_back(rref[i]);
    rref = vector<vector<int>>(M, vector<int>(M + 1));
    for (int i = 0; i < pruned_rref.size(); i++)
    {
        int lead = 0;
        while (lead < M && !pruned_rref[i][lead])
            lead++;
        rref[lead] = pruned_rref[i];
    }
    vector<int> fv, FV(M);
    for (int i = 0; i < M; i++)
        if (!rref[i][i])
        {
            FV[i] = fv.size() + 1;
            fv.push_back(i);
        }
    int R = fv.size();
    vector<vector<int>> sols(M, vector<int>(R + 1));
    for (int i = M - 1; i >= 0; i--)
    {
        if (FV[i])
            sols[i][FV[i]] = 1;
        else
        {
            sols[i][0] = rref[i][M];
            for (int j = i + 1; j < M; j++)
                for (int k = 0; k <= R; k++)
                    sols[i][k] = us(sols[i][k], rref[i][j] * sols[j][k]);
            for (int k = 0; k <= R; k++)
            {
                sols[i][k] *= inv(rref[i][i]);
                sols[i][k] %= MOD;
            }
        }
    }
    // R+1 * M matrix
    cout << R << endl;
    for (int j = 0; j <= R; j++)
    {
        for (int i = 0; i < M; i++)
            cout << sols[i][j] << " ";
        cout << endl;
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
