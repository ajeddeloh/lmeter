#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct Sine {
	const int16_t *data;
	size_t len;
} Sine;

extern const Sine SINES[5];
extern const size_t N_SINES;
