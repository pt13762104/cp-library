// @brief Matrix multiplication (Strassen)
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#define int unsigned
int size1, lgsz;
#define SZ_MAX 1024
#define BLOCK 128
using namespace std;
struct row {
  int val[SZ_MAX];
  int &operator[](int x) { return val[x]; }
};
struct mat {
  row val[SZ_MAX];
  row &operator[](int x) { return val[x]; }
};
struct row64 {
  unsigned long long val[BLOCK];
  unsigned long long &operator[](int x) { return val[x]; }
};
struct mat64 {
  row64 val[BLOCK];
  row64 &operator[](int x) { return val[x]; }
};
unsigned long long modmod8 = 8ULL * 998244353 * 998244353;
mat aa, bb, cc;
mat tmpmat[9][10];
#ifdef __clang__
int __lg(int x) {
  int cnt = 0;
  while (x) {
    x >>= 1;
    cnt++;
  }
  return --cnt;
}
#endif
int m32(int a) { return a % 998244353; }
int us(int a, int b) {
  if (a < b)
    return 998244353 - us(b, a);
  return m32(a - b);
}
void add03(mat &__restrict__ a, mat &__restrict__ res) {
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++) {
      res[i][j] = m32(a[i][j] + a[i + size1][j + size1]);
    }
}
void extract3(mat &__restrict__ a, mat &__restrict__ res) {
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++) {
      res[i][j] = m32(a[i + size1][j + size1]);
    }
}
void add23(mat &__restrict__ a, mat &__restrict__ res) {
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++) {
      res[i][j] = m32(a[i + size1][j] + a[i + size1][j + size1]);
    }
}
void add01(mat &__restrict__ a, mat &__restrict__ res) {
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++) {
      res[i][j] = m32(a[i][j] + a[i][j + size1]);
    }
}
void sub13(mat &__restrict__ a, mat &__restrict__ res) {
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++) {
      res[i][j] = us(a[i][j + size1], a[i + size1][j + size1]);
    }
}
void sub20(mat &__restrict__ a, mat &__restrict__ res) {
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++) {
      res[i][j] = us(a[i + size1][j], a[i][j]);
    }
}
void naive(mat &__restrict__ a, mat &__restrict__ b, mat &__restrict__ cx) {
  mat64 c;
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++)
      c[i][j] = 0;
  for (int i = 0; i < size1; i++)
    for (int k = 0; k < size1; k++) {
      for (int j = 0; j < size1; j++) {
        c[i][j] += (unsigned long long)(a[i][k]) * (b[k][j]);
      }
      if (k % 8 == 0)
        for (int j = 0; j < size1; j++)
          c[i][j] = min(c[i][j], c[i][j] - modmod8);
    }
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++)
      cx[i][j] = c[i][j] % 998244353;
}
void mul(mat &__restrict__ a, mat &__restrict__ b, mat &__restrict__ c) {
  if (size1 <= BLOCK) {
    naive(a, b, c);
    return;
  }
  size1 >>= 1;
  lgsz--;
  mat &__restrict__ m1 = tmpmat[0][lgsz];
  mat &__restrict__ m2 = tmpmat[1][lgsz];
  mat &__restrict__ m3 = tmpmat[2][lgsz];
  mat &__restrict__ m4 = tmpmat[3][lgsz];
  mat &__restrict__ m5 = tmpmat[4][lgsz];
  mat &__restrict__ m6 = tmpmat[5][lgsz];
  mat &__restrict__ m7 = tmpmat[6][lgsz];
  mat &__restrict__ m8 = tmpmat[7][lgsz];
  mat &__restrict__ m9 = tmpmat[8][lgsz];
  add03(a, m8);
  add03(b, m9);
  mul(m8, m9, m1);
  add23(a, m8);
  mul(m8, b, m2);
  sub13(b, m9);
  mul(a, m9, m3);
  extract3(a, m8);
  sub20(b, m9);
  mul(m8, m9, m4);
  add01(a, m8);
  extract3(b, m9);
  mul(m8, m9, m5);
  sub20(a, m8);
  add01(b, m9);
  mul(m8, m9, m6);
  sub13(a, m8);
  add23(b, m9);
  mul(m8, m9, m7);
  for (int i = 0; i < size1; i++)
    for (int j = 0; j < size1; j++) {
      c[i][j] = m32(us(m4[i][j], m5[i][j]) + m1[i][j] + m7[i][j]);
      c[i][j + size1] = m32(m3[i][j] + m5[i][j]);
      c[i + size1][j] = m32(m2[i][j] + m4[i][j]);
      c[i + size1][j + size1] =
          m32(us(m1[i][j], m2[i][j]) + m3[i][j] + m6[i][j]);
    }
  lgsz++;
  size1 <<= 1;
}
signed main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);
  int n, m, p;
  cin >> n >> m >> p;
  int mx = max(max(m, n), p);
  size1 = 1 << __lg(mx);
  if (size1 < mx)
    size1 <<= 1;
  lgsz = __lg(size1);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      cin >> aa[i][j];
  for (int i = 0; i < m; i++)
    for (int j = 0; j < p; j++)
      cin >> bb[i][j];
  mul(aa, bb, cc);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < p; j++)
      cout << cc[i][j] << " ";
  cout << endl;
}