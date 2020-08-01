#pragma once

#include <complex.h>

#include "sine.h"

// ADC channels for PA0+PA1 and PA2+PA3 (in diff. mode)
// Would be good to use any of the PC0-3 pins since they're on fast
// ADC channels, but the Disco board uses them for other things instead.
#define CHAN1 5 // PA0 = V-, PA1 = V+
#define CHAN2 7 // PA2 = V-, PA3 = V+

// 20 uS, so 1MS is plenty
#define ADC_SETUP_TIME_MS 1

void adc_init();

complex float adc_capture(size_t n_waves, const Sine *sine);
