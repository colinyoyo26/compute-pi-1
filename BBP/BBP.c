#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIGITS 30
#define TERMS 101

/* compute 16^exp (mod dem) */
uint32_t modulo(uint32_t exp, uint32_t dem)
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

double compute_serie(int32_t pos, int32_t idx)
{
    int32_t dem, exp, m;
    static uint32_t idx_table[4] = {1, 4, 5, 6};
    double frac = 0;

    m = idx_table[idx];

    for (int32_t i = 0; i < pos; i++) {
        dem = 8 * i + m;
        exp = pos - i;

        frac += (double) modulo(exp, dem) / (double) dem;
        frac -= (int) frac;
    }

    for (int32_t i = pos; i < pos + TERMS; i++) {
        dem = 8 * i + m;
        frac += pow(16., (double) (pos - i)) / (double) dem;
        frac -= (int) frac;
    }
    return frac;
}

void print_hex(double num)
{
    for (uint32_t i = 0; i < 10; i++) {
        num = 16. * (num - (int) num);
        printf("%c", "0123456789ABCDEF"[(int) num]);
    }
}

double compute_pi_BBP(int pos)
{
    double series[4], sum;

    series[0] = compute_serie(pos, 0);
    series[1] = compute_serie(pos, 1);
    series[2] = compute_serie(pos, 2);
    series[3] = compute_serie(pos, 3);
    sum = 4. * series[0] - 2. * series[1] - series[2] - series[3];
    sum = sum - (int) sum + 1.;
    return sum;
}