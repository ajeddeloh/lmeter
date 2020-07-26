#pragma once

#include "sine.h"

#define DAC_BUF_LEN 2
#define DAC_CYCLES_PER_UPDATE 15

void dac_init();
void dac_change_sine(const Sine *sine);
