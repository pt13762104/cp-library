// @brief Pollard rho factorization
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
#include <random>
using namespace std;
#define int unsigned long long
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int mult(int a, int b, int M) { return (__uint128_t)a * b % M; }
int f(int x, int c, int mod) { return (mult(x, x, mod) + c) % mod; }
int dist(int x, int y) { return (x < y ? y - x : x - y); }
int rho(int n, int x, int c) {
  if (!(n & 1))
    return 2;
  int iter = 100;
  int ox = x;
  int oy = x, y = x;
  int g = 1;
  int z = 1;
  while (g == 1) {
    for (int i = 0; i < iter; i++) {
      x = f(x, c, n);
      y = f(y, c, n);
      y = f(y, c, n);
      z = mult(z, dist(x, y), n);
      if (i == iter - 1)
        g = __gcd(z, n);
    }
    if (g == 1)
      oy = y, ox = x;
  }
  if (g == n) {
    x = ox;
    y = oy;
    g = 1;
    while (g == 1) {
      x = f(x, c, n);
      y = f(y, c, n);
      y = f(y, c, n);
      g = __gcd(dist(x, y), n);
    }
  }
  return g;
}
int bpow(int a, int b, int mod) {
  int res = 1;
  while (b) {
    if (b & 1)
      res = mult(res, a, mod);
    a = mult(a, a, mod);
    b >>= 1;
  }
  return res;
}
bool _mr(int n, int a) {
  int s = __builtin_ctzll(n - 1), d = (n - 1) >> s;
  int x = bpow(a, d, n);
  while (s--) {
    int y = mult(x, x, n);
    if (y == 1 && x != 1 && x != (n - 1))
      return 0;
    x = y;
  }
  return x == 1;
}
bool mr(int n) {
  // https://judge.yosupo.jp/submission/197324
  if (n < 64)
    return 0x28208a20a08a28ac >> n & 1;
  if ((n % 2) == 0 || (n % 3) == 0 || (n % 5) == 0 || (n % 7) == 0)
    return 0;
  vector<int> a = {2, 3, 61};
  if (n >> 32)
    a = {2, 325, 9375, 28178, 450775, 9780504};
  for (auto aa : a)
    if (aa < n - 1 && !_mr(n, aa))
      return 0;
  return 1;
}
void factor(int x, vector<int> &res) {
  if (x == 1)
    return;
  if (mr(x)) {
    res.push_back(x);
    return;
  }
  int x0 = rng();
  int a = rho(x, x0, 69);
  while (a == x)
    x0 = rng(), a = rho(x, x0, rng());
  factor(a, res);
  factor(x / a, res);
}
signed main() {
  int t;
  cin >> t;
  while (t--) {
    int k;
    cin >> k;
    vector<int> fs;
    factor(k, fs);
    sort(fs.begin(), fs.end());
    cout << fs.size() << " ";
    for (auto i : fs)
      cout << i << " ";
    cout << endl;
  }
}