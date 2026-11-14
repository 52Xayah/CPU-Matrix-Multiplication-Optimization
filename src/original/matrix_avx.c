#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <immintrin.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

float rand_float(float s) {
    return 4.0f * s * (1.0f - s);
}

void matrix_gen(float *a, float *b, int N, float seed) {
    float s = seed;
    for (int i = 0; i < N * N; i++) {
        s = rand_float(s);
        a[i] = s;
        s = rand_float(s);
        b[i] = s;
    }
}

void matrix_mul_avx(float *a, float *b, float *c, int N, int m) {
    if (m <= 0 || m > N) m = N;
    #pragma omp parallel for collapse(2) schedule(static)
    for (int ii = 0; ii < N; ii += m) {
        for (int jj = 0; jj < N; jj += m) {
            for (int kk = 0; kk < N; kk += m) {
                int i_end = MIN(ii + m, N);
                int j_end = MIN(jj + m, N);
                int k_end = MIN(kk + m, N);
                
                for (int i = ii; i < i_end; i++) {
                    for (int k = kk; k < k_end; k++) {
                        float r = a[i * N + k];
                        __m256 r_vec = _mm256_set1_ps(r);
                        
                        int j = jj;
                        for (; j <= j_end - 8; j += 8) {
                            __m256 c_vec = _mm256_loadu_ps(&c[i * N + j]);
                            __m256 b_vec = _mm256_loadu_ps(&b[k * N + j]);
                            __m256 prod = _mm256_mul_ps(r_vec, b_vec);
                            c_vec = _mm256_add_ps(c_vec, prod);
                            _mm256_storeu_ps(&c[i * N + j], c_vec);
                        }
                        
                        for (; j < j_end; j++) {
                            c[i * N + j] += r * b[k * N + j];
                        }
                    }
                }
            }
        }
    }
}

float calculate_trace(float *c, int N) {
    float trace = 0.0f;
    for (int i = 0; i < N; i++) {
        trace += c[i * N + i];
    }
    return trace;
}

int main(int argc, char *argv[]) {
    if (argc != 5) return -1;

    int N = atoi(argv[1]);
    float seed = atof(argv[2]);
    int m = atoi(argv[3]);
    int threads = atoi(argv[4]);

    omp_set_num_threads(threads);

    size_t size = (size_t)N * N * sizeof(float);
    float *A = (float *)malloc(size);
    float *B = (float *)malloc(size);
    float *C = (float *)calloc(N * N, sizeof(float));

    if (A == NULL || B == NULL || C == NULL) return -1;

    matrix_gen(A, B, N, seed);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    matrix_mul_avx(A, B, C, N, m);

    gettimeofday(&end, NULL);

    double time_used = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    float trace = calculate_trace(C, N);

    printf("N=%d, m=%d, Threads=%d, AVX\n", N, m, threads);
    printf("Trace: %f\n", trace);
    printf("Time: %f s\n", time_used);

    free(A);
    free(B);
    free(C);

    return 0;
}
