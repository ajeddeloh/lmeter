#pragma once

#include <complex.h>

#include "sine.h"

float complex get_impedance(const volatile int16_t *data, size_t len, const Sine *sine);
void linear_regression(size_t n, const float *xs, const float complex *ys, float complex *res_m,
		float complex *res_b);

