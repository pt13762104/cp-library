// @brief Matrix multiplication (Strassen)
#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#pragma GCC optimize("Ofast,unroll-loops")
#include <immintrin.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;
namespace fastio
{
static constexpr size_t buf_size = 1 << 18;
static constexpr size_t integer_size = 20;
static constexpr size_t block_size = 10000;

static char inbuf[buf_size + 1] = {};
static char outbuf[buf_size + 1] = {};
static char block_str[block_size * 4 + 1] = {};

static constexpr uint64_t power10[] = {1,
                                       10,
                                       100,
                                       1000,
                                       10000,
                                       100000,
                                       1000000,
                                       10000000,
                                       100000000,
                                       1000000000,
                                       10000000000,
                                       100000000000,
                                       1000000000000,
                                       10000000000000,
                                       100000000000000,
                                       1000000000000000,
                                       10000000000000000,
                                       100000000000000000,
                                       1000000000000000000,
                                       10000000000000000000u};

struct Scanner
{
  private:
    size_t pos, end;

    void load()
    {
        end = fread(inbuf, 1, buf_size, stdin);
        inbuf[end] = '\0';
    }
    void reload()
    {
        size_t len = end - pos;
        memmove(inbuf, inbuf + pos, len);
        end = len + fread(inbuf + len, 1, buf_size - len, stdin);
        inbuf[end] = '\0';
        pos = 0;
    }
    void skip_space()
    {
        while (inbuf[pos] <= ' ')
        {
            if (__builtin_expect(++pos == end, 0))
                reload();
        }
    }
    char get_next()
    {
        return inbuf[pos++];
    }
    char get_next_nonspace()
    {
        skip_space();
        return inbuf[pos++];
    }

  public:
    Scanner()
    {
        load();
    }

    void scan(char &c)
    {
        c = get_next_nonspace();
    }
    void scan(std::string &s)
    {
        skip_space();
        s = "";
        do
        {
            size_t start = pos;
            while (inbuf[pos] > ' ')
                pos++;
            s += std::string(inbuf + start, inbuf + pos);
            if (inbuf[pos] != '\0')
                break;
            reload();
        } while (true);
    }

    template <class T> typename std::enable_if<std::is_integral<T>::value, void>::type scan(T &x)
    {
        char c = get_next_nonspace();
        if (__builtin_expect(pos + integer_size >= end, 0))
            reload();
        bool neg = false;
        if (c == '-')
            neg = true, x = 0;
        else
            x = c & 15;
        while ((c = get_next()) >= '0')
            x = x * 10 + (c & 15);
        if (neg)
            x = -x;
    }

    template <class Head, class... Others> void scan(Head &head, Others &...others)
    {
        scan(head);
        scan(others...);
    }

    template <class T> Scanner &operator>>(T &x)
    {
        scan(x);
        return *this;
    }
};

struct Printer
{
  private:
    size_t pos = 0;

    void flush()
    {
        fwrite(outbuf, 1, pos, stdout);
        pos = 0;
    }

    void pre_calc()
    {
        for (size_t i = 0; i < block_size; i++)
        {
            size_t j = 4, k = i;
            while (j--)
            {
                block_str[i * 4 + j] = k % 10 + '0';
                k /= 10;
            }
        }
    }

    static constexpr size_t get_integer_size(uint64_t n)
    {
        if (n >= power10[10])
        {
            if (n >= power10[19])
                return 20;
            if (n >= power10[18])
                return 19;
            if (n >= power10[17])
                return 18;
            if (n >= power10[16])
                return 17;
            if (n >= power10[15])
                return 16;
            if (n >= power10[14])
                return 15;
            if (n >= power10[13])
                return 14;
            if (n >= power10[12])
                return 13;
            if (n >= power10[11])
                return 12;
            return 11;
        }
        else
        {
            if (n >= power10[9])
                return 10;
            if (n >= power10[8])
                return 9;
            if (n >= power10[7])
                return 8;
            if (n >= power10[6])
                return 7;
            if (n >= power10[5])
                return 6;
            if (n >= power10[4])
                return 5;
            if (n >= power10[3])
                return 4;
            if (n >= power10[2])
                return 3;
            if (n >= power10[1])
                return 2;
            return 1;
        }
    }

  public:
    Printer()
    {
        pre_calc();
    }
    ~Printer()
    {
        flush();
    }

    void print(char c)
    {
        outbuf[pos++] = c;
        if (__builtin_expect(pos == buf_size, 0))
            flush();
    }
    void print(const char *s)
    {
        while (*s != 0)
        {
            outbuf[pos++] = *s++;
            // if (pos == buf_size) flush();
            if (__builtin_expect(pos == buf_size, 0))
                flush();
        }
    }
    void print(const std::string &s)
    {
        for (auto c : s)
        {
            outbuf[pos++] = c;
            // if (pos == buf_size) flush();
            if (__builtin_expect(pos == buf_size, 0))
                flush();
        }
    }

    template <class T> typename std::enable_if<std::is_integral<T>::value, void>::type print(T x)
    {
        if (__builtin_expect(pos + integer_size >= buf_size, 0))
            flush();
        if (x < 0)
            print('-'), x = -x;
        size_t digit = get_integer_size(x);
        size_t len = digit;
        while (len >= 4)
        {
            len -= 4;
            memcpy(outbuf + pos + len, block_str + (x % block_size) * 4, 4);
            x /= block_size;
        }
        memcpy(outbuf + pos, block_str + x * 4 + (4 - len), len);
        pos += digit;
    }

    template <class Head, class... Others> void print(const Head &head, const Others &...others)
    {
        print(head);
        print(' ');
        print(others...);
    }

    template <class... Args> void println(const Args &...args)
    {
        print(args...);
        print('\n');
    }

    template <class T> Printer &operator<<(const T &x)
    {
        print(x);
        return *this;
    }
};
}; // namespace fastio

fastio::Scanner fin;
fastio::Printer fout;
const int SZ = 1024;
const int NAIVE = 128;
chrono::high_resolution_clock Clock;
int64_t modmod8 = 8ULL * 998244353 * 998244353;
alignas(32) unsigned A[SZ * SZ], B[SZ * SZ], C[SZ * SZ], TMPMAT[3][349528], *BUF = &TMPMAT[0][0];
alignas(32) uint64_t TMP[NAIVE * NAIVE];
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
const int s1 = 64, s3 = 32;
alignas(32) __m256i tmp[8 * s1];
alignas(32) uint64_t bb[NAIVE * NAIVE], cc[NAIVE * NAIVE];
const __m256i MM8 = {modmod8, modmod8, modmod8, modmod8};
uint64_t Accum_time = 0;
int calls = 0;
static inline __attribute__((always_inline)) __m256i shrink(__m256i a)
{
    __m256i mask = _mm256_cmpgt_epi64(a - MM8, __m256i{});
    return a - (MM8 & mask);
}
static inline __attribute__((always_inline)) void naive(const unsigned *a, const unsigned *B, unsigned *C, const int N)
{
    auto tp = Clock.now();
    memset(cc, 0, N * N * sizeof(uint64_t));
    for (int i = 0; i < N * N; i++)
        bb[i] = B[i];
    alignas(32) __m256i *b = (__m256i *)bb, *c = (__m256i *)cc;
    calls++;
    for (int i3 = 0; i3 < N; i3 += s3)
    {
        for (int i1 = 0; i1 < N; i1 += s1)
        {
            for (int k = i1; k < i1 + s1; k++)
            {
                tmp[8 * (k - i1) + 0] = b[k * N / 4 + i3 / 4 + 0];
                tmp[8 * (k - i1) + 1] = b[k * N / 4 + i3 / 4 + 1];
                tmp[8 * (k - i1) + 2] = b[k * N / 4 + i3 / 4 + 2];
                tmp[8 * (k - i1) + 3] = b[k * N / 4 + i3 / 4 + 3];
                tmp[8 * (k - i1) + 4] = b[k * N / 4 + i3 / 4 + 4];
                tmp[8 * (k - i1) + 5] = b[k * N / 4 + i3 / 4 + 5];
                tmp[8 * (k - i1) + 6] = b[k * N / 4 + i3 / 4 + 6];
                tmp[8 * (k - i1) + 7] = b[k * N / 4 + i3 / 4 + 7];
            }
            for (int i = 0; i < N; i++)
            {
                __m256i t0 = _mm256_load_si256(c + (i * N + i3) / 4 + 0);
                __m256i t1 = _mm256_load_si256(c + (i * N + i3) / 4 + 1);
                __m256i t2 = _mm256_load_si256(c + (i * N + i3) / 4 + 2);
                __m256i t3 = _mm256_load_si256(c + (i * N + i3) / 4 + 3);
                __m256i t4 = _mm256_load_si256(c + (i * N + i3) / 4 + 4);
                __m256i t5 = _mm256_load_si256(c + (i * N + i3) / 4 + 5);
                __m256i t6 = _mm256_load_si256(c + (i * N + i3) / 4 + 6);
                __m256i t7 = _mm256_load_si256(c + (i * N + i3) / 4 + 7);
                for (int k = 0; k < s1; k++)
                {
                    __m256i aik = _mm256_set1_epi32(a[i * N + (k + i1)]);
                    t0 = _mm256_add_epi64(t0, _mm256_mul_epu32(aik, tmp[8 * k + 0]));
                    t1 = _mm256_add_epi64(t1, _mm256_mul_epu32(aik, tmp[8 * k + 1]));
                    t2 = _mm256_add_epi64(t2, _mm256_mul_epu32(aik, tmp[8 * k + 2]));
                    t3 = _mm256_add_epi64(t3, _mm256_mul_epu32(aik, tmp[8 * k + 3]));
                    t4 = _mm256_add_epi64(t4, _mm256_mul_epu32(aik, tmp[8 * k + 4]));
                    t5 = _mm256_add_epi64(t5, _mm256_mul_epu32(aik, tmp[8 * k + 5]));
                    t6 = _mm256_add_epi64(t6, _mm256_mul_epu32(aik, tmp[8 * k + 6]));
                    t7 = _mm256_add_epi64(t7, _mm256_mul_epu32(aik, tmp[8 * k + 7]));
                    if (k % 8 == 7)
                    {
                        t0 = shrink(t0);
                        t1 = shrink(t1);
                        t2 = shrink(t2);
                        t3 = shrink(t3);
                        t4 = shrink(t4);
                        t5 = shrink(t5);
                        t6 = shrink(t6);
                        t7 = shrink(t7);
                    }
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
    fin >> n >> m >> p;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fin >> BUF[i * SZ + j];
    prep(A, BUF, 0, 0, 0, SZ);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < p; j++)
            fin >> BUF[i * SZ + j];
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
            fout << BUF[i * SZ + j];
            fout << (j == p - 1 ? '\n' : ' ');
        }
}
