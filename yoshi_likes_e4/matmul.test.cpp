// @brief Matrix multiplication (AVX2)
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#pragma GCC optimize("O3,unroll-loops")
#include <chrono>
#include <immintrin.h>
#include <iostream>
using namespace std;
#ifndef yoshi_likes_e4
#define endl '\n'
#endif
#define problem ""
#define multitest 0
#define debug(x) cerr << #x << " = " << x << endl;
alignas(32) int32_t a[1048576];
alignas(32) uint64_t bb[1048576], cc[1048576];
alignas(32) __m256i *b = (__m256i *)bb, *c = (__m256i *)cc;
alignas(32) __m256i tmp[8192];
alignas(32) __m256i t0, t1, t2, t3, t4, t5, t6, t7;
void init()
{
}
// https://stackoverflow.com/questions/54394350/simd-implement-mm256-max-epu64-and-mm256-min-epu64
static inline __attribute__((always_inline)) __m256i pmin_epu64(__m256i a, __m256i b)
{
    __m256i signbit = _mm256_set1_epi64x(0x8000'0000'0000'0000);
    __m256i mask = _mm256_cmpgt_epi64(_mm256_xor_si256(a, signbit), _mm256_xor_si256(b, signbit));
    return _mm256_blendv_epi8(a, b, mask);
}
using namespace chrono;
high_resolution_clock Clock;
const int s1 = 128, s2 = 64, s3 = 32;
const uint64_t MM8 = 8ULL * 998244353 * 998244353;
const int n = 1024;
void Yoshi()
{
    int n_, m_, k_;
    cin >> n_ >> m_ >> k_;
    for (int i = 0; i < n_; i++)
        for (int j = 0; j < m_; j++)
            cin >> a[i * n + j];
    for (int i = 0; i < m_; i++)
        for (int j = 0; j < k_; j++)
            cin >> bb[i * n + j];
    auto tx = Clock.now();
    for (int i3 = 0; i3 < 1024; i3 += s3)
    {
        for (int k = 0; k < 1024; k++)
        {
            tmp[8 * k + 0] = b[k * n / 4 + i3 / 4 + 0];
            tmp[8 * k + 1] = b[k * n / 4 + i3 / 4 + 1];
            tmp[8 * k + 2] = b[k * n / 4 + i3 / 4 + 2];
            tmp[8 * k + 3] = b[k * n / 4 + i3 / 4 + 3];
            tmp[8 * k + 4] = b[k * n / 4 + i3 / 4 + 4];
            tmp[8 * k + 5] = b[k * n / 4 + i3 / 4 + 5];
            tmp[8 * k + 6] = b[k * n / 4 + i3 / 4 + 6];
            tmp[8 * k + 7] = b[k * n / 4 + i3 / 4 + 7];
        }
        for (int i1 = 0; i1 < 1024; i1 += s1)
            for (int i2 = 0; i2 < 1024; i2 += s2)
                for (int i = i2; i < i2 + s2; i++)
                {
                    t0 = _mm256_load_si256(c + (i * n + i3) / 4 + 0);
                    t1 = _mm256_load_si256(c + (i * n + i3) / 4 + 1);
                    t2 = _mm256_load_si256(c + (i * n + i3) / 4 + 2);
                    t3 = _mm256_load_si256(c + (i * n + i3) / 4 + 3);
                    t4 = _mm256_load_si256(c + (i * n + i3) / 4 + 4);
                    t5 = _mm256_load_si256(c + (i * n + i3) / 4 + 5);
                    t6 = _mm256_load_si256(c + (i * n + i3) / 4 + 6);
                    t7 = _mm256_load_si256(c + (i * n + i3) / 4 + 7);
                    for (int k = i1; k < i1 + s1; k++)
                    {
                        __m256i aik = __m256i{} + a[i * n + k];
                        if (k % 8 == 0)
                        {
                            t0 = pmin_epu64(t0, t0 - MM8);
                            t1 = pmin_epu64(t1, t1 - MM8);
                            t2 = pmin_epu64(t2, t2 - MM8);
                            t3 = pmin_epu64(t3, t3 - MM8);
                            t4 = pmin_epu64(t4, t4 - MM8);
                            t5 = pmin_epu64(t5, t5 - MM8);
                            t6 = pmin_epu64(t6, t6 - MM8);
                            t7 = pmin_epu64(t7, t7 - MM8);
                        }
                        t0 = _mm256_add_epi64(t0, _mm256_mul_epi32(aik, tmp[8 * k + 0]));
                        t1 = _mm256_add_epi64(t1, _mm256_mul_epi32(aik, tmp[8 * k + 1]));
                        t2 = _mm256_add_epi64(t2, _mm256_mul_epi32(aik, tmp[8 * k + 2]));
                        t3 = _mm256_add_epi64(t3, _mm256_mul_epi32(aik, tmp[8 * k + 3]));
                        t4 = _mm256_add_epi64(t4, _mm256_mul_epi32(aik, tmp[8 * k + 4]));
                        t5 = _mm256_add_epi64(t5, _mm256_mul_epi32(aik, tmp[8 * k + 5]));
                        t6 = _mm256_add_epi64(t6, _mm256_mul_epi32(aik, tmp[8 * k + 6]));
                        t7 = _mm256_add_epi64(t7, _mm256_mul_epi32(aik, tmp[8 * k + 7]));
                    }
                    _mm256_store_si256(c + (i * n + i3) / 4 + 0, t0);
                    _mm256_store_si256(c + (i * n + i3) / 4 + 1, t1);
                    _mm256_store_si256(c + (i * n + i3) / 4 + 2, t2);
                    _mm256_store_si256(c + (i * n + i3) / 4 + 3, t3);
                    _mm256_store_si256(c + (i * n + i3) / 4 + 4, t4);
                    _mm256_store_si256(c + (i * n + i3) / 4 + 5, t5);
                    _mm256_store_si256(c + (i * n + i3) / 4 + 6, t6);
                    _mm256_store_si256(c + (i * n + i3) / 4 + 7, t7);
                }
    }
    auto t2x = Clock.now();
    for (int i = 0; i < n_; i++)
    {
        for (int j = 0; j < k_; j++)
            cout << cc[i * n + j] % 998244353 << " ";
        cout << endl;
    }
    cerr << "Time: " << duration_cast<milliseconds>(t2x - tx).count() << " ms" << endl;
    cerr << "GIOPS: " << (2 * 1024 * 1024 * 1024.0) / duration_cast<nanoseconds>(t2x - tx).count() << endl;
}
signed main()
{
#ifndef yoshi_likes_e4
    ios::sync_with_stdio(0);
    cin.tie(0);
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