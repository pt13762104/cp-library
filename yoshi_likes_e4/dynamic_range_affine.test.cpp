// @brief Dynamic Sequence Range Affine Range Linear Add Range Sum
#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
using namespace std;
#define int long long
const int mod = 998244353;
const uint64_t seed =
    chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937_64 rng(seed);
typedef struct item *pitem;
struct item
{
    uint64_t prior;
    int init;
    int step;
    int mul;
    int add;
    int f;
    int value;
    int cnt;
    bool rev;
    item(int x)
    {
        value = f = x % mod;
        step = add = init = rev = 0;
        prior = rng();
        cnt = mul = 1;
        l = nullptr;
        r = nullptr;
    }
    pitem l, r;
};
int cnt(pitem it) { return it ? it->cnt : 0; }
int f(pitem it) { return it ? it->f : 0; }
int ssum(int in, int ad, int k)
{
    // Sum of the first k terms starting from in, added by ad each
    return ((in * k) + (ad * ((k * k - k) >> 1) % mod)) % mod;
}
void push(pitem &it)
{
    if (it)
    {
        if (it->rev)
        {
            it->rev = false;
            swap(it->l, it->r);
            if (it->l)
                it->l->rev ^= true;
            if (it->r)
                it->r->rev ^= true;
        }
        it->f *= it->mul;
        it->f += it->add * cnt(it);
        it->f += ssum(it->init, it->step, cnt(it));
        it->f %= mod;
        it->value *= it->mul;
        it->value += it->add;
        it->value += it->init + it->step * cnt(it->l);
        it->value %= mod;
        if (it->l)
        {
            it->l->mul *= it->mul;
            it->l->add *= it->mul;
            it->l->init *= it->mul;
            it->l->step *= it->mul;
            it->l->add += it->add;
            it->l->init += it->init;
            it->l->step += it->step;
            it->l->mul %= mod;
            it->l->add %= mod;
            it->l->init %= mod;
            it->l->step %= mod;
        }
        if (it->r)
        {
            it->r->mul *= it->mul;
            it->r->add *= it->mul;
            it->r->init *= it->mul;
            it->r->step *= it->mul;
            it->r->add += it->add;
            it->r->init += it->init + it->step * (cnt(it->l) + 1);
            it->r->step += it->step;
            it->r->mul %= mod;
            it->r->add %= mod;
            it->r->init %= mod;
            it->r->step %= mod;
        }
        it->mul = 1;
        it->add = it->init = it->step = 0;
    }
}
void up(pitem it)
{
    if (it)
    {
        push(it->l);
        push(it->r);
        it->cnt = cnt(it->l) + cnt(it->r) + 1;
        it->f = (it->value + f(it->l) + f(it->r)) % mod;
    }
}
pitem merge(pitem l, pitem r)
{
    push(l);
    push(r);
    if (!l || !r)
        return (l ? l : r);
    else if (l->prior > r->prior)
    {
        l->r = merge(l->r, r);
        up(l);
        return l;
    }
    else
    {
        r->l = merge(l, r->l);
        up(r);
        return r;
    }
}

pair<pitem, pitem> split(pitem t, int key, int add = 0)
{
    if (!t)
        return {0, 0};
    push(t);
    int cur_key = add + cnt(t->l);
    if (key <= cur_key)
    {
        auto z = split(t->l, key, add);
        t->l = z.second;
        up(t);
        return {z.first, t};
    }
    else
    {
        auto z = split(t->r, key, cur_key + 1);
        t->r = z.first;
        up(t);
        return {t, z.second};
    }
}
void insert(pitem &t, int pos, int x)
{
    int value = 0;
    pitem t1 = new item(x);
    auto sp = split(t, pos);
    t1->value = x;
    sp.second = merge(t1, sp.second);
    t = merge(sp.first, sp.second);
}
void erase(pitem &t, int pos)
{
    auto sp = split(t, pos);
    auto spp = split(sp.second, 1).second;
    t = merge(sp.first, spp);
}
int sum(pitem &t, int l, int r)
{
    int value = 0;
    auto sp = split(t, l);
    auto spp = split(sp.second, r - l + 1);
    value = f(spp.first);
    sp.second = merge(spp.first, spp.second);
    t = merge(sp.first, sp.second);
    return value;
}
void reverse(pitem &t, int l, int r)
{
    int value = 0;
    auto sp = split(t, l);
    auto spp = split(sp.second, r - l + 1);
    spp.first->rev ^= true;
    sp.second = merge(spp.first, spp.second);
    t = merge(sp.first, sp.second);
}
int affine(pitem &t, int l, int r, int A, int b)
{
    int value = 0;
    auto sp = split(t, l);
    auto spp = split(sp.second, r - l + 1);
    spp.first->add *= A;
    spp.first->mul *= A;
    spp.first->init *= A;
    spp.first->step *= A;
    spp.first->add += b;
    spp.first->add %= mod;
    spp.first->mul %= mod;
    spp.first->init %= mod;
    spp.first->step %= mod;
    sp.second = merge(spp.first, spp.second);
    t = merge(sp.first, sp.second);
    return value;
}
int linear_add(pitem &t, int l, int r, int A, int b)
{
    int value = 0;
    auto sp = split(t, l);
    auto spp = split(sp.second, r - l + 1);
    spp.first->init += A;
    spp.first->step += b;
    spp.first->init %= mod;
    spp.first->step %= mod;
    sp.second = merge(spp.first, spp.second);
    t = merge(sp.first, sp.second);
    return value;
}
signed main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, q;
    cin >> n >> q;
    pitem root = 0;
    for (int i = 0; i < n; i++)
    {
        int k;
        cin >> k;
        insert(root, i, k);
    }
    while (q--)
    {
        int t;
        cin >> t;
        switch (t)
        {
        case 2:
        {
            int l, r;
            cin >> l >> r;
            r--;
            reverse(root, l, r);
            break;
        }
        case 3:
        {
            int l, r, A, b;
            cin >> l >> r >> A >> b;
            r--;
            affine(root, l, r, A, b);
            break;
        }
        case 0:
        {
            int p, X;
            cin >> p >> X;
            insert(root, p, X);
            break;
        }
        case 1:
        {
            int p;
            cin >> p;
            erase(root, p);
            break;
        }
        default:
        {
            int l, r;
            cin >> l >> r;
            r--;
            cout << sum(root, l, r) << endl;
        }
        }
    }
}