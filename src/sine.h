#pragma once

#include <stdint.h>

extern const int16_t sine512[512];
extern const int16_t sine256[256];
extern const int16_t sine128[128];
extern const int16_t sine64[64];

#define n_sine (sizeof(sine)/sizeof(sine[0]))
