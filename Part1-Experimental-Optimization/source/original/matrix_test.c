#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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

void matrix_mul(float *a, float *b, float *c, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0f;
            for (int k = 0; k < N; k++) {
                sum += a[i * N + k] * b[k * N + j];
            }
            c[i * N + j] = sum;
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
    if (argc != 3) {
        printf("Usage: %s <N> <seed>\n", argv[0]);
        return -1;
    }

    int N = atoi(argv[1]);
    float seed = atof(argv[2]);

    size_t size = (size_t)N * N * sizeof(float);
    float *A = (float *)malloc(size);
    float *B = (float *)malloc(size);
    float *C = (float *)malloc(size);

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    matrix_gen(A, B, N, seed);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    matrix_mul(A, B, C, N);

    gettimeofday(&end, NULL);

    double time_used = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    float trace = calculate_trace(C, N);

    printf("Trace: %f\n", trace);
    printf("Time: %f s\n", time_used);

    free(A);
    free(B);
    free(C);

    return 0;
}
