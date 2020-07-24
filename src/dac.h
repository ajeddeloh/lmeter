#pragma once

#include "sine.h"

#define DAC_BUF_LEN 2
#define DAC_CYCLES_PER_UPDATE 15

void init_dac();
void dac_change_sine(const Sine *sine);
