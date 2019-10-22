#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0

extern double compute_pi_BBP(int pos);
extern double compute_pi_BBP_openmp(int pos, int threads);

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};
    const size_t loop = 20;

    clock_gettime(CLOCK_ID, &start);
    for (size_t i = 0; i < loop; i++) {
        compute_pi_BBP(N);
    }
    clock_gettime(CLOCK_ID, &end);
    printf("%lf ", (double) (end.tv_sec - start.tv_sec) +
                       (end.tv_nsec - start.tv_nsec) / ONE_SEC);

    clock_gettime(CLOCK_ID, &start);
    for (size_t i = 0; i < loop; i++) {
        compute_pi_BBP_openmp(N, 2);
    }
    clock_gettime(CLOCK_ID, &end);
    printf("%lf ", (double) (end.tv_sec - start.tv_sec) +
                       (end.tv_nsec - start.tv_nsec) / ONE_SEC);

    clock_gettime(CLOCK_ID, &start);
    for (size_t i = 0; i < loop; i++) {
        compute_pi_BBP_openmp(N, 4);
    }
    clock_gettime(CLOCK_ID, &end);
    printf("%lf\n", (double) (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / ONE_SEC);

    return 0;
}
