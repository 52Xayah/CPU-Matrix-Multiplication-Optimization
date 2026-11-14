#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <omp.h>

#define MC 64
#define NC 128
#define KC 256
#define MR 4
#define NR 16

static float rand_float(float s) { return 4 * s * (1 - s); }

static void matrix_gen(float *a, float *b, int N, float seed) {
    float s = seed;
    long n2 = (long)N * N;
    for (long i = 0; i < n2; i++) {
        s = rand_float(s); a[i] = s;
        s = rand_float(s); b[i] = s;
    }
}
static inline void kern4x16(int kc, const float *a, int lda, const float *bp, float *c) {
    __m256 c00 = _mm256_load_ps(c),            c01 = _mm256_load_ps(c + 8);
    __m256 c10 = _mm256_load_ps(c + NC),       c11 = _mm256_load_ps(c + NC + 8);
    __m256 c20 = _mm256_load_ps(c + 2 * NC),   c21 = _mm256_load_ps(c + 2 * NC + 8);
    __m256 c30 = _mm256_load_ps(c + 3 * NC),   c31 = _mm256_load_ps(c + 3 * NC + 8);
    const float *a0 = a, *a1 = a + lda, *a2 = a + 2 * (long)lda, *a3 = a + 3 * (long)lda;
    for (int k = 0; k < kc; k++) {
        __m256 b0 = _mm256_load_ps(bp), b1 = _mm256_load_ps(bp + 8);
        bp += NR;
        __m256 x;
        x = _mm256_broadcast_ss(a0 + k);
        c00 = _mm256_add_ps(c00, _mm256_mul_ps(x, b0)); c01 = _mm256_add_ps(c01, _mm256_mul_ps(x, b1));
        x = _mm256_broadcast_ss(a1 + k);
        c10 = _mm256_add_ps(c10, _mm256_mul_ps(x, b0)); c11 = _mm256_add_ps(c11, _mm256_mul_ps(x, b1));
        x = _mm256_broadcast_ss(a2 + k);
        c20 = _mm256_add_ps(c20, _mm256_mul_ps(x, b0)); c21 = _mm256_add_ps(c21, _mm256_mul_ps(x, b1));
        x = _mm256_broadcast_ss(a3 + k);
        c30 = _mm256_add_ps(c30, _mm256_mul_ps(x, b0)); c31 = _mm256_add_ps(c31, _mm256_mul_ps(x, b1));
    }
    _mm256_store_ps(c, c00);            _mm256_store_ps(c + 8, c01);
    _mm256_store_ps(c + NC, c10);       _mm256_store_ps(c + NC + 8, c11);
    _mm256_store_ps(c + 2 * NC, c20);   _mm256_store_ps(c + 2 * NC + 8, c21);
    _mm256_store_ps(c + 3 * NC, c30);   _mm256_store_ps(c + 3 * NC + 8, c31);
}
static inline void kern_edge(int mr, int kc, const float *a, int lda, const float *bp, float *c) {
    for (int r = 0; r < mr; r++) {
        __m256 c0 = _mm256_load_ps(c + r * NC), c1 = _mm256_load_ps(c + r * NC + 8);
        const float *ar = a + (long)r * lda;
        const float *b = bp;
        for (int k = 0; k < kc; k++) {
            __m256 x = _mm256_broadcast_ss(ar + k);
            c0 = _mm256_add_ps(c0, _mm256_mul_ps(x, _mm256_load_ps(b)));
            c1 = _mm256_add_ps(c1, _mm256_mul_ps(x, _mm256_load_ps(b + 8)));
            b += NR;
        }
        _mm256_store_ps(c + r * NC, c0); _mm256_store_ps(c + r * NC + 8, c1);
    }
}
int main(int argc, char **argv) {
    if (argc < 3) return 1;
    int N = atoi(argv[1]);
    float seed = (float)atof(argv[2]);
    if (N <= 0) return 1;

    long n2 = (long)N * N;
    float *A, *B;
    if (posix_memalign((void **)&A, 64, n2 * sizeof(float))) return 1;
    if (posix_memalign((void **)&B, 64, n2 * sizeof(float))) return 1;
    float *diag = (float *)malloc(N * sizeof(float));

    matrix_gen(A, B, N, seed);

    int nI = (N + MC - 1) / MC, nJ = (N + NC - 1) / NC;
    long ntiles = (long)nI * nJ;

    #pragma omp parallel
    {
        float *Bp, *Ct;
        if (posix_memalign((void **)&Bp, 64, KC * NC * sizeof(float))) exit(1);
        if (posix_memalign((void **)&Ct, 64, MC * NC * sizeof(float))) exit(1);

        #pragma omp for schedule(dynamic, 1)
        for (long t = 0; t < ntiles; t++) {
            int ic = (int)(t / nJ) * MC, jc = (int)(t % nJ) * NC;
            int mc = N - ic < MC ? N - ic : MC;
            int nc = N - jc < NC ? N - jc : NC;
            int npan = (nc + NR - 1) / NR;

            for (int i = 0; i < MC * NC; i++) Ct[i] = 0.0f;

            for (int k0 = 0; k0 < N; k0 += KC) {
                int kc = N - k0 < KC ? N - k0 : KC;
                for (int p = 0; p < npan; p++) {
                    int j0 = p * NR;
                    int w = nc - j0 < NR ? nc - j0 : NR;
                    float *dst = Bp + (long)p * kc * NR;
                    for (int k = 0; k < kc; k++) {
                        const float *src = B + (long)(k0 + k) * N + jc + j0;
                        int j = 0;
                        for (; j < w; j++) dst[j] = src[j];
                        for (; j < NR; j++) dst[j] = 0.0f;
                        dst += NR;
                    }
                }
                for (int p = 0; p < npan; p++) {
                    const float *bp = Bp + (long)p * kc * NR;
                    float *cp = Ct + p * NR;
		    int ir = 0;
                    for (; ir + MR <= mc; ir += MR)
                        kern4x16(kc, A + (long)(ic + ir) * N + k0, N, bp, cp + ir * NC);
                    if (ir < mc)
                        kern_edge(mc - ir, kc, A + (long)(ic + ir) * N + k0, N, bp, cp + ir * NC);
                }
            }
            for (int i = ic; i < ic + mc; i++)
                if (i >= jc && i < jc + nc) diag[i] = Ct[(i - ic) * NC + (i - jc)];
        }
        free(Bp); free(Ct);
    }

    float trace = 0.0f;
    for (int i = 0; i < N; i++) trace += diag[i];
    printf("%f\n", trace);
    return 0;
}

