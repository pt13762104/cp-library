// @brief Matrix multiplication (mod 2)
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product_mod_2"
#pragma GCC optimize("O3,unroll-loops")
#include <chrono>
#include <cstring>
#include <iostream>
#include <stdio.h>
#define getchar getchar_unlocked
#define putchar putchar_unlocked
using namespace std;
#define endl '\n'
#define debug(x) cerr << #x << " = " << x << endl;
alignas(32) long long a[4096][64];
alignas(32) long long b[4096][64];
alignas(32) long long c[4096][64];
alignas(32) long long TMP[256][64];
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
int main()
{
    chrono::high_resolution_clock Clock;
    auto t1 = Clock.now();
    int n, m, k;
    n = rd(), m = rd(), k = rd();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            a[i][j >> 6] |= rd_bool() * (1LL << (j & 63));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < k; j++)
            b[i][j >> 6] |= rd_bool() * (1LL << (j & 63));
    cerr << "Input time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
    t1 = Clock.now();
    for (int j = 0; j < 4096; j += 8)
    {
        for (int bit = 0; bit < 8; bit++)
            for (int cur = 0; cur < (1 << bit); cur++)
                for (int k = 0; k < 64; k++)
                    TMP[cur + (1 << bit)][k] = TMP[cur][k] ^ b[j + bit][k];
        for (int i = 0; i < 4096; i += 256)
            for (int X = 0; X < 256; X++)
            {
                int mask = (a[i + X][j >> 6] >> (j & 63)) & 255;
                if (mask)
                    for (int k = 0; k < 64; k++)
                        c[i + X][k] ^= TMP[mask][k];
            }
    }
    cerr << "Compute time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
    t1 = Clock.now();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
            putchar((((c[i][j >> 6]) >> (j & 63)) & 1) + '0');
        putchar('\n');
    }
    cerr << "Output time: " << chrono::duration_cast<chrono::milliseconds>(Clock.now() - t1).count() << " ms" << endl;
}
