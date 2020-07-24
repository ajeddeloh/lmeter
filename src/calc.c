#include <math.h>
#include <complex.h>
#include <stdint.h>

#include "calc.h"

#define RESISTOR_VALUE 1000.0f

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

// get_impedance calculates the impendance of the inductor based on the ADC readings in data.
// The two ADC channels are interleaved in data. sine is the sine wave at the same frequency
// we are applying.
float complex get_impedance(const volatile int16_t *data, size_t len, const Sine *sine) {
        // Compute sum( f(t) * e^(i*omega) ) for the the adc data at the frequency we're
        // measuring. Use int64_t since we could overflow 32 bits. Convert to float for the
        // math later, but keep them as ints while adding to preserve precision
        // TODO: use a seperate set of more precise sine tables for this rather than the dac
        // ones.
        int64_t i_real = 0;
        int64_t i_imag = 0;
        int64_t t_real = 0;
        int64_t t_imag = 0;
        for (size_t i = 0; i < len; i+=2) {
                int32_t tot = data[i];
                int32_t ind = data[i + 1];
                int32_t sin = sine->data_high_res[(i*2) % sine->len];
                int32_t cos = sine->data_high_res[(i*2 + sine->len/4) % sine->len];
                i_real += ind*cos;
                i_imag -= ind*sin;
                t_real += tot*cos;
                t_imag -= tot*sin;
        }
        float complex inductor = i_real + I * i_imag;
        float complex total = t_real + I * t_imag;
        float complex impedance = (inductor/total)*RESISTOR_VALUE/(1-(inductor/total));
        return impedance;
}
