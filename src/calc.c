#include <math.h>
#include <complex.h>
#include <stdint.h>

#include "calc.h"

// Linear regression takes a list of xs and a list of ys (of size n) and does simple
// linear regression to determine the best fit line. It returns the slope in res_m and the
// y intercept in res_b.
void linear_regression(size_t n, const float *xs, const float complex *ys,
                float complex *res_m, float complex *res_b) {
        float complex numerator = 0;
        float complex denominator = 0;
        float complex mean_x = 0;
        float complex mean_y = 0;
        for (size_t i = 0; i < n; i++) {
                mean_x += xs[i];
                mean_y += ys[i];
        }
        mean_x /= n;
        mean_y /= n;
        for (size_t i = 0; i < n; i++) {
                numerator += (xs[i]-mean_x)*(ys[i]-mean_y);
                denominator += (xs[i]-mean_x)*(xs[i]-mean_x);
        }
        *res_m = numerator/denominator;
        *res_b = mean_y-(*res_m) * mean_x;
}
