// @brief Matrix multiplication (Strassen)
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#pragma GCC optimize(3)
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")
#include <chrono>
#include <cstring>
#include <immintrin.h>
#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;
namespace fastio
{
static constexpr int SZ = 1 << 17;
char inbuf[SZ], outbuf[SZ];
int in_left = 0, in_right = 0, out_right = 0;

struct Pre
{
    char num[40000];
    constexpr Pre() : num()
    {
        for (int i = 0; i < 10000; i++)
        {
            int n = i;
            for (int j = 3; j >= 0; j--)
            {
                num[i * 4 + j] = n % 10 + '0';
                n /= 10;
            }
        }
    }
} constexpr pre;

inline void load()
{
    int len = in_right - in_left;
    memmove(inbuf, inbuf + in_left, len);
    in_right = len + fread(inbuf + len, 1, SZ - len, stdin);
    in_left = 0;
}

inline void flush()
{
    fwrite(outbuf, 1, out_right, stdout);
    out_right = 0;
}

inline void skip_space()
{
    if (in_left + 32 > in_right)
        load();
    while (inbuf[in_left] <= ' ')
        in_left++;
}

inline void rd(char &c)
{
    if (in_left + 32 > in_right)
        load();
    c = inbuf[in_left++];
}
template <typename T> inline void rd(T &x)
{
    if (in_left + 32 > in_right)
        load();
    char c;
    do
        c = inbuf[in_left++];
    while (c < '-');
    [[maybe_unused]] bool minus = false;
    if constexpr (is_signed<T>::value == true)
    {
        if (c == '-')
            minus = true, c = inbuf[in_left++];
    }
    x = 0;
    while (c >= '0')
    {
        x = x * 10 + (c & 15);
        c = inbuf[in_left++];
    }
    if constexpr (is_signed<T>::value == true)
    {
        if (minus)
            x = -x;
    }
}
inline void rd()
{
}
template <typename Head, typename... Tail> inline void rd(Head &head, Tail &...tail)
{
    rd(head);
    rd(tail...);
}

inline void wt(char c)
{
    if (out_right > SZ - 32)
        flush();
    outbuf[out_right++] = c;
}
inline void wt(bool b)
{
    if (out_right > SZ - 32)
        flush();
    outbuf[out_right++] = b ? '1' : '0';
}
template <typename T> inline void wt(T x)
{
    if (out_right > SZ - 32)
        flush();
    if (!x)
    {
        outbuf[out_right++] = '0';
        return;
    }
    if constexpr (is_signed<T>::value == true)
    {
        if (x < 0)
            outbuf[out_right++] = '-', x = -x;
    }
    int i = 12;
    char buf[16];
    while (x >= 10000)
    {
        memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
        x /= 10000;
        i -= 4;
    }
    if (x < 100)
    {
        if (x < 10)
        {
            outbuf[out_right] = '0' + x;
            ++out_right;
        }
        else
        {
            uint32_t q = (uint32_t(x) * 205) >> 11;
            uint32_t r = uint32_t(x) - q * 10;
            outbuf[out_right] = '0' + q;
            outbuf[out_right + 1] = '0' + r;
            out_right += 2;
        }
    }
    else
    {
        if (x < 1000)
        {
            memcpy(outbuf + out_right, pre.num + (x << 2) + 1, 3);
            out_right += 3;
        }
        else
        {
            memcpy(outbuf + out_right, pre.num + (x << 2), 4);
            out_right += 4;
        }
    }
    memcpy(outbuf + out_right, buf + i + 4, 12 - i);
    out_right += 12 - i;
}
inline void wt()
{
}
template <typename Head, typename... Tail> inline void wt(Head &&head, Tail &&...tail)
{
    wt(head);
    wt(forward<Tail>(tail)...);
}
template <typename... Args> inline void wtn(Args &&...x)
{
    wt(forward<Args>(x)...);
    wt('\n');
}

struct Dummy
{
    Dummy()
    {
        atexit(flush);
    }
} dummy;

} // namespace fastio
using fastio::rd;
using fastio::skip_space;
using fastio::wt;
using fastio::wtn;
const int SZ = 1024;
const int NAIVE = 128;
chrono::high_resolution_clock Clock;
uint64_t modmod8 = 8ULL * 998244353 * 998244353;
alignas(32) unsigned BUF[SZ * SZ], A[SZ * SZ], B[SZ * SZ], C[SZ * SZ], TMPMAT[3][(SZ * SZ - 1) / 3];
alignas(32) uint64_t TMP[NAIVE * NAIVE];
#ifdef __clang__
int __lg(int x)
{
    int cnt = 0;
    while (x)
    {
        x >>= 1;
        cnt++;
    }
    return --cnt;
}
#endif
inline int ua(const unsigned a, const unsigned b)
{
    return min(a + b, a + b - 998244353);
}
inline int us(const unsigned a, const unsigned b)
{
    return min(a - b, a + 998244353 - b);
}
void add03(const unsigned *a, unsigned *res, const int N)
{
    for (int i = 0; i < N * N; i++)
        res[i] = ua(a[i], a[3 * N * N + i]);
}
void extract0(const unsigned *a, unsigned *res, const int N)
{
    for (int i = 0; i < N * N; i++)
        res[i] = a[i];
}
void extract3(const unsigned *a, unsigned *res, const int N)
{
    for (int i = 0; i < N * N; i++)
        res[i] = a[3 * N * N + i];
}
void add23(const unsigned *a, unsigned *res, const int N)
{
    for (int i = 0; i < N * N; i++)
        res[i] = ua(a[2 * N * N + i], a[3 * N * N + i]);
}
void add01(const unsigned *a, unsigned *res, const int N)
{
    for (int i = 0; i < N * N; i++)
        res[i] = ua(a[i], a[N * N + i]);
}
void sub13(const unsigned *a, unsigned *res, const int N)
{
    for (int i = 0; i < N * N; i++)
        res[i] = us(a[N * N + i], a[3 * N * N + i]);
}
void sub20(const unsigned *a, unsigned *res, const int N)
{
    for (int i = 0; i < N * N; i++)
        res[i] = us(a[2 * N * N + i], a[i]);
}
const int s1 = 128, s2 = 64, s3 = 32;
alignas(32) __m256i tmp[8 * NAIVE];
alignas(32) __m256i t0, t1, t2, t3, t4, t5, t6, t7;
alignas(32) uint64_t bb[NAIVE * NAIVE], cc[NAIVE * NAIVE];
uint64_t Accum_time = 0;
int calls = 0;
// https://stackoverflow.com/questions/54394350/simd-implement-mm256-max-epu64-and-mm256-min-epu64
static inline __attribute__((always_inline)) __m256i pmin_epu64(__m256i a, __m256i b)
{
    __m256i signbit = _mm256_set1_epi64x(0x8000'0000'0000'0000);
    __m256i mask = _mm256_cmpgt_epi64(_mm256_xor_si256(a, signbit), _mm256_xor_si256(b, signbit));
    return _mm256_blendv_epi8(a, b, mask);
}
void naive(const unsigned *a, const unsigned *B, unsigned *C, const int N)
{
    auto tp = Clock.now();
    memset(cc, 0, N * N * sizeof(uint64_t));
    for (int i = 0; i < N * N; i++)
        bb[i] = B[i];
    alignas(32) __m256i *b = (__m256i *)bb, *c = (__m256i *)cc;
    calls++;
    for (int i3 = 0; i3 < N; i3 += s3)
    {
        for (int k = 0; k < N; k++)
        {
            tmp[8 * k + 0] = b[k * N / 4 + i3 / 4 + 0];
            tmp[8 * k + 1] = b[k * N / 4 + i3 / 4 + 1];
            tmp[8 * k + 2] = b[k * N / 4 + i3 / 4 + 2];
            tmp[8 * k + 3] = b[k * N / 4 + i3 / 4 + 3];
            tmp[8 * k + 4] = b[k * N / 4 + i3 / 4 + 4];
            tmp[8 * k + 5] = b[k * N / 4 + i3 / 4 + 5];
            tmp[8 * k + 6] = b[k * N / 4 + i3 / 4 + 6];
            tmp[8 * k + 7] = b[k * N / 4 + i3 / 4 + 7];
        }
        for (int i1 = 0; i1 < N; i1 += s1)
            for (int i2 = 0; i2 < N; i2 += s2)
                for (int i = i2; i < i2 + s2; i++)
                {
                    t0 = _mm256_load_si256(c + (i * N + i3) / 4 + 0);
                    t1 = _mm256_load_si256(c + (i * N + i3) / 4 + 1);
                    t2 = _mm256_load_si256(c + (i * N + i3) / 4 + 2);
                    t3 = _mm256_load_si256(c + (i * N + i3) / 4 + 3);
                    t4 = _mm256_load_si256(c + (i * N + i3) / 4 + 4);
                    t5 = _mm256_load_si256(c + (i * N + i3) / 4 + 5);
                    t6 = _mm256_load_si256(c + (i * N + i3) / 4 + 6);
                    t7 = _mm256_load_si256(c + (i * N + i3) / 4 + 7);
                    for (int k = i1; k < i1 + s1; k++)
                    {
                        __m256i aik = __m256i{} + a[i * N + k];
                        if (k % 8 == 0)
                        {
                            t0 = pmin_epu64(t0, t0 - modmod8);
                            t1 = pmin_epu64(t1, t1 - modmod8);
                            t2 = pmin_epu64(t2, t2 - modmod8);
                            t3 = pmin_epu64(t3, t3 - modmod8);
                            t4 = pmin_epu64(t4, t4 - modmod8);
                            t5 = pmin_epu64(t5, t5 - modmod8);
                            t6 = pmin_epu64(t6, t6 - modmod8);
                            t7 = pmin_epu64(t7, t7 - modmod8);
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
                    _mm256_store_si256(c + (i * N + i3) / 4 + 0, t0);
                    _mm256_store_si256(c + (i * N + i3) / 4 + 1, t1);
                    _mm256_store_si256(c + (i * N + i3) / 4 + 2, t2);
                    _mm256_store_si256(c + (i * N + i3) / 4 + 3, t3);
                    _mm256_store_si256(c + (i * N + i3) / 4 + 4, t4);
                    _mm256_store_si256(c + (i * N + i3) / 4 + 5, t5);
                    _mm256_store_si256(c + (i * N + i3) / 4 + 6, t6);
                    _mm256_store_si256(c + (i * N + i3) / 4 + 7, t7);
                }
    }
    for (int i = 0; i < N * N; i++)
        C[i] = cc[i] % 998244353;
    Accum_time += chrono::duration_cast<chrono::nanoseconds>(Clock.now() - tp).count();
}
void mul(const unsigned *a, const unsigned *b, unsigned *c, const int N)
{
    memset(c, 0, N * N * sizeof(unsigned));
    if (N == NAIVE)
    {
        naive(a, b, c, N);
        return;
    }
    const int offset = (SZ * SZ - N * N) / 3;
    unsigned *m1 = TMPMAT[0] + offset;
    unsigned *m2 = TMPMAT[1] + offset;
    unsigned *m3 = TMPMAT[2] + offset;
    add03(a, m2, N / 2);
    add03(b, m3, N / 2);
    mul(m2, m3, m1, N / 2);
    memcpy(c, m1, N * N * sizeof(unsigned) / 4);
    memcpy(c + 3 * N * N / 4, m1, N * N * sizeof(unsigned) / 4);
    add23(a, m2, N / 2);
    extract0(b, m3, N / 2);
    mul(m2, m3, m1, N / 2);
    memcpy(c + N * N / 2, m1, N * N * sizeof(unsigned) / 4);
    for (int i = 0; i < N * N / 4; i++)
        c[i + 3 * N * N / 4] = us(c[i + 3 * N * N / 4], m1[i]);
    sub13(b, m3, N / 2);
    extract0(a, m2, N / 2);
    mul(m2, m3, m1, N / 2);
    for (int i = 0; i < N * N / 4; i++)
        c[i + N * N / 4] = ua(c[i + N * N / 4], m1[i]);
    for (int i = 0; i < N * N / 4; i++)
        c[i + 3 * N * N / 4] = ua(c[i + 3 * N * N / 4], m1[i]);
    extract3(a, m2, N / 2);
    sub20(b, m3, N / 2);
    mul(m2, m3, m1, N / 2);
    for (int i = 0; i < N * N / 4; i++)
        c[i] = ua(c[i], m1[i]);
    for (int i = 0; i < N * N / 4; i++)
        c[i + N * N / 2] = ua(c[i + N * N / 2], m1[i]);
    add01(a, m2, N / 2);
    extract3(b, m3, N / 2);
    mul(m2, m3, m1, N / 2);
    for (int i = 0; i < N * N / 4; i++)
        c[i] = us(c[i], m1[i]);
    for (int i = 0; i < N * N / 4; i++)
        c[i + N * N / 4] = ua(c[i + N * N / 4], m1[i]);
    sub20(a, m2, N / 2);
    add01(b, m3, N / 2);
    mul(m2, m3, m1, N / 2);
    for (int i = 0; i < N * N / 4; i++)
        c[i + 3 * N * N / 4] = ua(c[i + 3 * N * N / 4], m1[i]);
    sub13(a, m2, N / 2);
    add23(b, m3, N / 2);
    mul(m2, m3, m1, N / 2);
    for (int i = 0; i < N * N / 4; i++)
        c[i] = ua(c[i], m1[i]);
}
void prep(unsigned *dst, const unsigned *src, int ofs1, int ofs2, int ofs3, int N)
{
    if (N == NAIVE)
    {
        for (int i = 0; i < NAIVE; i++)
            for (int j = 0; j < NAIVE; j++)
                dst[ofs3 + i * NAIVE + j] = src[(i + ofs1) * SZ + j + ofs2];
        return;
    }
    prep(dst, src, ofs1, ofs2, ofs3, N / 2);
    prep(dst, src, ofs1, ofs2 + N / 2, ofs3 + N * N / 4, N / 2);
    prep(dst, src, ofs1 + N / 2, ofs2, ofs3 + N * N / 2, N / 2);
    prep(dst, src, ofs1 + N / 2, ofs2 + N / 2, ofs3 + 3 * N * N / 4, N / 2);
}
void prep_reverse(unsigned *dst, const unsigned *src, int ofs1, int ofs2, int ofs3, int N)
{
    if (N == NAIVE)
    {
        for (int i = 0; i < NAIVE; i++)
            for (int j = 0; j < NAIVE; j++)
                dst[(i + ofs1) * SZ + j + ofs2] = src[ofs3 + i * NAIVE + j];
        return;
    }
    prep_reverse(dst, src, ofs1, ofs2, ofs3, N / 2);
    prep_reverse(dst, src, ofs1, ofs2 + N / 2, ofs3 + N * N / 4, N / 2);
    prep_reverse(dst, src, ofs1 + N / 2, ofs2, ofs3 + N * N / 2, N / 2);
    prep_reverse(dst, src, ofs1 + N / 2, ofs2 + N / 2, ofs3 + 3 * N * N / 4, N / 2);
}
signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m, p;
    rd(n);
    rd(m);
    rd(p);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            rd(BUF[i * SZ + j]);
    prep(A, BUF, 0, 0, 0, SZ);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < p; j++)
            rd(BUF[i * SZ + j]);
    prep(B, BUF, 0, 0, 0, SZ);
    auto t1 = Clock.now();
    mul(A, B, C, SZ);
    cerr << "Compute time: " << chrono::duration_cast<chrono::nanoseconds>(Clock.now() - t1).count() << "ns" << endl;
    prep_reverse(BUF, C, 0, 0, 0, SZ);
    cerr << "Accum_time: " << Accum_time << "ns" << endl;
    cerr << "Calls: " << calls << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
        {
            wt(BUF[i * SZ + j]);
            wt(j == p - 1 ? '\n' : ' ');
        }
}
