#pragma once

#include <stdint.h>
#include <stdlib.h>

#define MAX_SINE_LEN 512
#define SINE_DEC 8
#define N_WAVES 32

typedef struct Sine {
	// data for the DAC to use. It's 4x longer since the DAC is update 4x as
	// frequency as the DAC samples.
	uint16_t data[MAX_SINE_LEN];
	// sin and cos for doing calcuations in the DFT
	int32_t sin_high_res[MAX_SINE_LEN/4];
	int32_t cos_high_res[MAX_SINE_LEN/4];
	// 2 * pi * frequency for this sine
	float omega;
	// number of elements in the DAC data. ADC data will have 4x fewer
	size_t len;
} Sine;

void sine_populate();
const Sine *get_sine(int idx);
