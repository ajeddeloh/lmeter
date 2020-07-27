#include <math.h>

#include "sine.h"

#define OFFSET 2048
#define AMP 1024
#define AMP_HIGH_RES 0x7FFFF

#define M_PI 3.14159265358979323846264338327f

static Sine sine = {
	.data = {0},
	.data_high_res = {0},
	.len = 0,
};

Sine *get_sine(size_t len) {
	if (len == sine.len) {
		return &sine;
	}
	for (size_t i = 0; i < len; i++) {
		sine.data[i] = OFFSET + round(sinf(2.0*i*M_PI/len)*AMP);
		sine.data_high_res[i] = round(sinf(2.0*i*M_PI/len)*AMP_HIGH_RES);
	}
	sine.len = len;
	return &sine;
}
