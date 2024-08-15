// @brief System of Linear Equations
#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
#include <bits/stdc++.h>
using namespace std;
#define int long long
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
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
vector<vector<int>> __ref(vector<vector<int>> x)
{
    int h = 0, k = 0;
    while (h < x.size() && k < x[0].size())
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
                {
                    x[i][j] = us(x[i][j], coeff * x[h][j]);
                }
            }
            h++;
        }
        k++;
    }
    return x;
}
pair<vector<vector<int>>, vector<int>> convert_to_rref(vector<vector<int>> ref)
{
    // Convert a system to the normal form
    for (auto &i : ref)
    {
        if (count(i.begin(), i.end() - 1, 0) == i.size() - 1 && i.back())
        {
            // No solution
            return {};
        }
    }
    int N = ref[0].size() - 1;
    while (ref.size() < N)
        ref.push_back(vector<int>(N + 1));
    while (ref.size() > N)
        ref.pop_back();
    vector<vector<int>> tmp(N, vector<int>(N + 1));
    for (int i = 0; i < N; i++)
        tmp[i][N] = i;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            tmp[i][j] = ref[j][i];
    vector<vector<int>> nf, f;
    for (int i = 0; i < N; i++)
        if (count(tmp[i].begin(), tmp[i].end() - 1, 0) == N)
            f.push_back(tmp[i]);
        else
            nf.push_back(tmp[i]);
    tmp = nf;
    for (auto &i : f)
        tmp.push_back(i);
    vector<int> P(N);
    for (int i = 0; i < N; i++)
        P[tmp[i][N]] = i;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            ref[i][j] = tmp[j][i];
    for (int i = 0; i < N; i++)
    {
        if (ref[i][i] != 0)
        {
            int coeff = inv(ref[i][i]);
            for (int j = i; j <= N; j++)
            {
                ref[i][j] *= coeff;
                ref[i][j] %= MOD;
            }
        }
    }
    return {ref, P};
}
void init()
{
}
void Yoshi()
{
    int N, M;
    cin >> N >> M;
    vector<vector<int>> raw(N, vector<int>(M + 1));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> raw[i][j];
    for (int i = 0; i < N; i++)
        cin >> raw[i][M];
    auto [rref, P] = convert_to_rref(__ref(raw));
    if (!rref.size())
    {
        cout << -1 << endl;
        return;
    }
    // rref is now [M, M+1]
    // the rank of solution is the number of zero rows [free variables]
    int R = 0;
    for (int i = 0; i < M; i++)
        R += (count(rref[i].begin(), rref[i].end(), 0) == M + 1);
    cout << R << endl;
    // get an arbitary solution [with the free variables set to 0]
    vector<int> sols(M);
    // consider the first M - R rows
    for (int i = M - R - 1; i >= 0; i--)
    {
        sols[i] = rref[i][M];
        for (int j = i + 1; j < M; j++)
            sols[i] = us(sols[i], rref[i][j] * sols[j]);
    }
    for (int i = 0; i < M; i++)
        cout << sols[P[i]] << " ";
    cout << endl;
    // get the basis
    for (int k = M - R; k < M; k++)
    {
        vector<int> tmp(M);
        tmp[k] = 1;
        for (int i = M - R - 1; i >= 0; i--)
            for (int j = i + 1; j < M; j++)
                tmp[i] = us(tmp[i], rref[i][j] * tmp[j]);
        for (int i = 0; i < M; i++)
            cout << tmp[P[i]] << " ";
        cout << endl;
    }
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