#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DIGITS 30
#define TERMS 101
int thread_num = 1;

/* compute 16^exp (mod dem) */
uint32_t modulo_omp(uint32_t exp, uint32_t dem)
{
    uint32_t gp;
    /* find greatest pow of 2 <= exp */
    if (dem == 1)
        return 0;
    for (gp = 0; exp >> gp; gp++)
        ;
    uint32_t pow2 = 1 << (gp - 1), r = 1;

    for (uint32_t j = 0; j < gp; j++) {
        if (exp >= pow2) {
            r <<= 4;
            r %= dem;
            exp -= pow2;
        }
        pow2 >>= 1;
        if (pow2) {
            r *= r;
            r %= dem;
        }
    }
    return r;
}

double compute_serie_omp(int32_t pos, int32_t idx)
{
    int32_t dem, exp, m;
    static uint32_t idx_table[4] = {1, 4, 5, 6};
    double frac = 0;

    m = idx_table[idx];
#pragma omp parallel num_threads(thread_num)
    {
#pragma omp for private(dem, exp) reduction(+ : frac)
        for (int32_t i = 0; i < pos; i++) {
            dem = 8 * i + m;
            exp = pos - i;

            frac += (double) modulo_omp(exp, dem) / (double) dem;
            frac -= (int) frac;
        }

#pragma omp for private(dem) reduction(+ : frac)
        for (int32_t i = pos; i < pos + TERMS; i++) {
            dem = 8 * i + m;
            frac += pow(16., (double) (pos - i)) / (double) dem;
            frac -= (int) frac;
        }
    }
    return frac;
}

void print_hex_omp(double num)
{
    for (uint32_t i = 0; i < 10; i++) {
        num = 16. * (num - (int) num);
        printf("%c", "0123456789ABCDEF"[(int) num]);
    }
}

double compute_pi_BBP_openmp(int pos, int threads)
{
    double series, sum = 0;
    double coe[4] = {4., -2., -1., -1.};
    thread_num = threads;

#pragma omp parallel num_threads(thread_num)
    {
#pragma omp for private(series) reduction(+ : sum)
        for (size_t i = 0; i < 4; i++) {
            series = compute_serie_omp(pos, i) * coe[i];
            sum += series;
        }
    }
    sum = sum - (int) sum + 1.;
    return sum;
}