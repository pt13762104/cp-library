// @brief Longest Increasing Subsequence using data structures
// https://cp-algorithms.com/sequences/longest_increasing_subsequence.html#solution-in-on-log-n-with-data-structures
#define PROBLEM "https://judge.yosupo.jp/problem/longest_increasing_subsequence"
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include <set>
using namespace std;
#define int long long
int argmax(int i, int j, vector<int> &a)
{
    if (i == -1 || j == -1)
        return max(i, j);
    return a[i] < a[j] ? j : i;
}
void build(int i, int l, int r, vector<int> &a, vector<int> &nodes)
{
    if (l == r)
    {
        nodes[i] = l;
        return;
    }
    else
    {
        int mi = (l + r) >> 1;
        build(i * 2 + 1, l, mi, a, nodes);
        build(i * 2 + 2, mi + 1, r, a, nodes);
        nodes[i] = argmax(nodes[i * 2 + 1], nodes[i * 2 + 2], a);
    }
}
void up(int i, int u, int v, int pos, int val, vector<int> &a, vector<int> &nodes)
{
    if (u > pos || v < pos)
        return;
    if (u == v && v == pos)
        a[u] = val;
    else
    {
        int mi = (u + v) >> 1;
        if (pos <= mi)
            up((i << 1) + 1, u, mi, pos, val, a, nodes);
        else
            up((i << 1) + 2, mi + 1, v, pos, val, a, nodes);
        nodes[i] = argmax(nodes[(i << 1) + 1], nodes[(i << 1) + 2], a);
    }
}
int get(int i, int u, int v, int l, int r, vector<int> &a, vector<int> &nodes)
{
    if (u > r || v < l)
        return -1;
    if (l <= u && v <= r)
        return nodes[i];
    else
    {
        int mi = (u + v) >> 1;
        return argmax(get((i << 1) + 1, u, mi, l, r, a, nodes), get((i << 1) + 2, mi + 1, v, l, r, a, nodes), a);
    }
}
template <typename T>
vector<T> lis(vector<T> x)
{
    set<T> ss;
    for (auto i : x)
        ss.insert(i);
    map<T, int> mp;
    int cnt = 0;
    for (auto i : ss)
    {
        mp[i] = cnt;
        cnt++;
    }
    vector<int> lis_real(x.size());
    for (int i = 0; i < x.size(); i++)
        lis_real[i] = mp[x[i]];
    vector<int> dp(ss.size()), nodes(4 * ss.size()), trace(x.size(), -1);
    build(0, 0, ss.size() - 1, dp, nodes);
    vector<vector<int>> idx(ss.size());
    int prefmin = ss.size();
    for (int i = 0; i < x.size(); i++)
    {
        idx[lis_real[i]].push_back(i);
        prefmin = min(prefmin, lis_real[i]);
        /* main lis code */
        int ii = get(0, 0, ss.size() - 1, 0, lis_real[i] - 1, dp, nodes);
        up(0, 0, ss.size() - 1, lis_real[i], (ii != -1 ? dp[ii] : 0) + 1, dp, nodes);
        /* end */
        trace[i] = (prefmin != lis_real[i] && ii != -1 ? idx[ii].back() : -1);
    }
    int amax = idx[get(0, 0, ss.size() - 1, 0, ss.size() - 1, dp, nodes)].back();
    vector<int> res;
    while (amax != -1)
    {
        res.push_back(amax);
        amax = trace[amax];
    }
    reverse(res.begin(), res.end());
    return res;
}
signed main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; i++)
        cin >> p[i];
    auto k = lis<int>(p);
    cout << k.size() << endl;
    for (auto i : k)
        cout << i << " ";
    cout << endl;
}