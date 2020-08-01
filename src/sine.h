#pragma once

#include <stdint.h>
#include <stdlib.h>

#define MAX_SINE_LEN 512

typedef struct Sine {
	uint16_t data[MAX_SINE_LEN];
	int32_t sin_high_res[MAX_SINE_LEN];
	int32_t cos_high_res[MAX_SINE_LEN];
	size_t len;
} Sine;

Sine *get_sine(size_t len);
