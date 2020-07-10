#include <math.h>

#include "sine.h"

#define OFFSET 2048
#define AMP 1024

#define M_PI 3.14159265358979323846264338327f

static Sine sine = {
	.data = {0},
	.len = 0,
};

Sine *get_sine(size_t len) {
	if (len == sine.len) {
		return &sine;
	}
	for (size_t i = 0; i < len; i++) {
		sine.data[i] = OFFSET + sinf(2.0*i*M_PI/len)*AMP;
	}
	sine.len = len;
	return &sine;
}
