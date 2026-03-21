// @brief Convolution $F_{2^{64}}$ (VPCLMULQDQ)
#define PROBLEM "https://judge.yosupo.jp/problem/convolution_F_2_64"
#include <bits/stdc++.h>
#include <immintrin.h>
namespace fastio
{
static constexpr u_int32_t buf_size = 1 << 17;
char ibuf[buf_size];
char obuf[buf_size];
u_int32_t pil = 0, pir = 0, por = 0;
struct Pre
{
    char t[40000];
    constexpr Pre()
    {
        for (int i = 0; i < 10000; i++)
        {
            int n = i;
            for (int j = 3; j >= 0; j--)
            {
                t[i * 4 + j] = '0' + n % 10;
                n /= 10;
            }
        }
    }
} constexpr pre;
inline void load()
{
    if (pir - pil <= pil)
        memcpy(ibuf, ibuf + pil, pir - pil);
    else
        memmove(ibuf, ibuf + pil, pir - pil);
    pir = pir - pil + fread(ibuf + pir - pil, 1, buf_size - pir + pil, stdin);
    pil = 0;
}
inline void flush()
{
    fwrite(obuf, 1, por, stdout);
    por = 0;
}
inline void rd(char &c)
{
    c = ibuf[pil++];
}
inline void rd(std::string &s)
{
    char c;
    s.clear();
    if (pil == pir)
        load();
    rd(c);
    if (pil == pir)
        load();
    while (!std::isspace(c))
    {
        s += c;
        rd(c);
        if (pil == pir)
            load();
    }
}
inline void rd(__int128_t &x)
{
    if (pil + 50 > pir)
        load();
    char c;
    do
        rd(c);
    while (c < '-');
    bool neg = false;
    if (c == '-')
    {
        neg = true;
        rd(c);
    }
    x = 0;
    while (c >= '0')
    {
        x = x * 10 + (c & 15);
        rd(c);
    }
    if (neg)
        x = -x;
}
template <typename T> inline void rd(T &x)
{
    if (pil + 32 > pir)
        load();
    char c;
    do
        rd(c);
    while (c < '-');
    bool neg = false;
    if constexpr (std::is_signed_v<T>)
    {
        if (c == '-')
        {
            neg = true;
            rd(c);
        }
    }
    x = 0;
    while (c >= '0')
    {
        x = x * 10 + (c & 15);
        rd(c);
    }
    if constexpr (std::is_signed_v<T>)
    {
        if (neg)
            x = -x;
    }
}
inline void wt(char x)
{
    obuf[por++] = x;
}
template <typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr> inline void wt(T x)
{
    if (por + 32 > buf_size)
        flush();
    if (x == 0)
    {
        wt('0');
        return;
    }
    if constexpr (std::is_signed_v<T>)
    {
        if (x < 0)
        {
            wt('-');
            x = -x;
        }
    }
    if (x >= 10000000000000000)
    {
        u_int32_t r1 = x % 100000000;
        u_int64_t q1 = x / 100000000;
        u_int32_t r2 = q1 % 100000000;
        u_int32_t q2 = q1 / 100000000;
        u_int32_t n1 = r1 % 10000, n2 = r1 / 10000, n3 = r2 % 10000, n4 = r2 / 10000;
        if (x >= 1000000000000000000)
        {
            if constexpr (std::is_unsigned_v<T>)
            {
                if (x >= 10000000000000000000ull)
                {
                    obuf[por++] = '1';
                }
            }
            memcpy(obuf + por, pre.t + (q2 << 2) + 1, 3);
            memcpy(obuf + por + 3, pre.t + (n4 << 2), 4);
            memcpy(obuf + por + 7, pre.t + (n3 << 2), 4);
            memcpy(obuf + por + 11, pre.t + (n2 << 2), 4);
            memcpy(obuf + por + 15, pre.t + (n1 << 2), 4);
            por += 19;
        }
        else if (x >= 100000000000000000)
        {
            u_int32_t q3 = (q2 * 205) >> 11;
            u_int32_t r3 = q2 - q3 * 10;
            obuf[por] = '0' + q3;
            obuf[por + 1] = '0' + r3;
            memcpy(obuf + por + 2, pre.t + (n4 << 2), 4);
            memcpy(obuf + por + 6, pre.t + (n3 << 2), 4);
            memcpy(obuf + por + 10, pre.t + (n2 << 2), 4);
            memcpy(obuf + por + 14, pre.t + (n1 << 2), 4);
            por += 18;
        }
        else
        {
            obuf[por] = '0' + q2;
            memcpy(obuf + por + 1, pre.t + (n4 << 2), 4);
            memcpy(obuf + por + 5, pre.t + (n3 << 2), 4);
            memcpy(obuf + por + 9, pre.t + (n2 << 2), 4);
            memcpy(obuf + por + 13, pre.t + (n1 << 2), 4);
            por += 17;
        }
    }
    else
    {
        static char buf[12];
        int i = 8;
        while (x >= 10000)
        {
            memcpy(buf + i, pre.t + ((x % 10000) << 2), 4);
            x /= 10000;
            i -= 4;
        }
        if (x < 100)
        {
            if (x < 10)
                obuf[por++] = '0' + x;
            else
            {
                obuf[por] = '0' + x / 10;
                obuf[por + 1] = '0' + x % 10;
                por += 2;
            }
        }
        else
        {
            if (x < 1000)
            {
                memcpy(obuf + por, pre.t + (x << 2) + 1, 3);
                por += 3;
            }
            else
            {
                memcpy(obuf + por, pre.t + (x << 2), 4);
                por += 4;
            }
        }
        memcpy(obuf + por, buf + i + 4, 8 - i);
        por += 8 - i;
    }
}
inline void wt(const std::string &s)
{
    int sz = s.size();
    if (por + sz > buf_size)
        flush();
    memcpy(obuf + por, s.c_str(), sz);
    por += sz;
}
inline void wt(__int128_t x)
{
    if (por + 50 > buf_size)
        flush();
    if (x == 0)
    {
        wt('0');
        return;
    }
    if (x < 0)
    {
        wt('-');
        x = -x;
    }
    static constexpr __int128_t b = 10000000000000000000ull;
    if (x >= b)
    {
        wt<unsigned long long>(x / b);
        unsigned long long y = x % b;
        memcpy(obuf + por, pre.t + ((y / 10000000000000000) << 2) + 1, 3);
        memcpy(obuf + por + 3, pre.t + ((y / 1000000000000 % 10000) << 2), 4);
        memcpy(obuf + por + 7, pre.t + ((y / 100000000 % 10000) << 2), 4);
        memcpy(obuf + por + 11, pre.t + ((y / 10000 % 10000) << 2), 4);
        memcpy(obuf + por + 15, pre.t + ((y % 10000) << 2), 4);
        por += 19;
    }
    else
        wt<unsigned long long>(x);
}
struct Dummy
{
    Dummy()
    {
        std::atexit(flush);
    }
} dummy;
} // namespace fastio
using fastio::rd;
using fastio::wt;
using namespace std;
#pragma GCC optimize("O3,unroll-loops")
alignas(32) uint64_t A[1 << 19];
alignas(32) uint64_t B[1 << 19];
alignas(32) uint64_t _t0[1 << 19], _t1[1 << 19];
alignas(32) uint64_t C[1 << 20], _t2[1 << 20];
static constexpr unsigned long long table[16] = {0,   27,  45,  54,  90,  65,  119, 108,
                                                 175, 180, 130, 153, 245, 238, 216, 195};
template <int m> void karatsuba(uint64_t *__restrict__ a, uint64_t *__restrict__ b, uint64_t *__restrict__ c)
{
    if (m == 8)
    {
        alignas(32) __m128i _tmp[16] = {};
        alignas(32) __m128i _tmp2[16] = {};
        for (int i = 0; i < m; i += 2)
        {
            __m256i _a = {(long long)(a[i]), 0, (long long)(a[i + 1]), 0};
            for (int j = 0; j < m; j += 2)
            {
                __m256i _x =
                    __m256i{(long long)(b[j]), (long long)(b[j + 1]), (long long)(b[j]), (long long)(b[j + 1])};
                *(__m256i *)(&_tmp[i + j]) ^= _mm256_clmulepi64_epi128(_a, _x, 0);
                *(__m256i *)(&_tmp2[i + j]) ^= _mm256_clmulepi64_epi128(_a, _x, 16);
            }
        }
        for (int i = 0; i < 2 * m - 1; i++)
        {
            auto TMP = _tmp[i];
            if (i)
                TMP ^= _tmp2[i - 1];
            uint64_t y = TMP[1] ^ (TMP[1] << 1);
            uint64_t x = TMP[0] ^ y ^ (y << 3);
            c[i] ^= x ^ table[(uint64_t)TMP[1] >> 60];
        }
    }
    else
    {
        karatsuba<m / 2>(a, b, c);
        karatsuba<m / 2>(a + (m >> 1), b + (m >> 1), c + m);
        memset(_t2 + (1 << 20) - 2 * m, 0, m * sizeof(uint64_t));
        for (int i = 0; i < (m >> 1); i++)
            _t0[(1 << 19) - m + i] = a[i] ^ a[i + (m >> 1)];
        for (int i = 0; i < (m >> 1); i++)
            _t1[(1 << 19) - m + i] = b[i] ^ b[i + (m >> 1)];
        karatsuba<m / 2>(_t0 + (1 << 19) - m, _t1 + (1 << 19) - m, _t2 + (1 << 20) - 2 * m);
        uint64_t *M = _t2 + (1 << 20) - 2 * m;
        for (int i = 0; i < (m >> 1); i++)
        {
            auto &A1 = c[i], &A2 = c[i + (m >> 1)], &A3 = c[i + m], &A4 = c[i + 3 * (m >> 1)];
            auto X = A2 ^ A3;
            A2 = X ^ M[i] ^ A1;
            A3 = X ^ M[i + (m >> 1)] ^ A4;
        }
    }
}
signed main()
{
    volatile int warm = 0;
    for (int i = 0; i < 100000; i++) warm += i;
    int N, M;
    rd(N), rd(M);
    for (int i = 0; i < N; i++)
        rd(A[i]);
    for (int i = 0; i < M; i++)
        rd(B[i]);
    int m = 1 << __lg(max(N, M));
    if (m < max(N, M))
        m <<= 1;
    karatsuba<524288>(A, B, C);
    for (int i = 0; i < N + M - 1; i++)
        wt(C[i]), wt(" \n"[i == N + M - 2]);
}