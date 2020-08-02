#include <math.h>

#include "sine.h"
#include "dac.h"

#define OFFSET 2048
#define AMP 1024
#define AMP_HIGH_RES 0x7FFFF

#define M_PI 3.14159265358979323846264338327f

static Sine sines[N_WAVES];

static void calc_sine(Sine *sine, size_t len);

void sine_populate() {
	for (int i = 0; i < N_WAVES; i++) {
		calc_sine(sines + i, MAX_SINE_LEN - i * SINE_DEC);
	}
}

const Sine *get_sine(int idx) {
	return sines + idx;
}

static void calc_sine(Sine *sine, size_t len) {
	for (size_t i = 0; i < len; i++) {
		sine->data[i] = OFFSET + round(sinf(2.0f*i*M_PI/len)*AMP);
	}
	for (size_t i = 0; i < len/4; i++) {
		sine->sin_high_res[i] = round(sinf(8.0f*i*M_PI/len)*AMP_HIGH_RES);
		sine->cos_high_res[i] = round(sinf(M_PI/2+8.0f*i*M_PI/len)*AMP_HIGH_RES);
	}
	sine->len = len;
	sine->omega = 2.0f*M_PI*80e6f/(DAC_CYCLES_PER_UPDATE*len);
}
