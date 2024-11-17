// @brief 64-ary tree
#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
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
void Yoshi()
{
    const int B = 64;
    vector<vector<uint64_t>> tree;
    int n, old_n;
    cin >> n;
    old_n = n;
    while (n > 1)
    {
        // n = ceil(n/B)
        tree.push_back(vector<uint64_t>(n = (n + B - 1) / B));
    }
    int h = tree.size();
    // remember: tree[i][k][j] means that at the jth subtree of tree[i][k] has sum > 0
    // This is copied from noimi
    auto insert = [&tree](int i) {
        // log_B(n)
        for (auto &v : tree)
        {
            v[i / B] |= 1ULL << (i % B);
            i /= B;
        }
    };
    auto erase = [&tree](int i) {
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
    auto get = [&tree](int i) { return (tree[0][i / B] >> (i % B)) & 1; };
    auto next = [&tree, h](int i) {
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
        return -1;
    };
    auto prev = [&tree, h](int i) {
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
    int q;
    cin >> q;
    string s;
    cin >> s;
    for (int i = 0; i < old_n; i++)
        if (s[i] - 48)
            insert(i);
    while (q--)
    {
        int t, x;
        cin >> t >> x;
        switch (t)
        {
        case 0: {
            insert(x);
            break;
        }
        case 1: {
            erase(x);
            break;
        }
        case 2: {
            cout << get(x) << endl;
            break;
        }
        case 3: {
            cout << next(x) << endl;
            break;
        }
        default: {
            cout << prev(x) << endl;
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