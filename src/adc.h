#pragma once

// ADCs will be running in master/slave configuration, which means
// their data will be written together after each conversion. Each
// set of conversions is 4 bytes, so this should be 64k of data total
#define ADC_BUF_LEN 16384

// 20 uS, so 1MS is plenty
#define ADC_SETUP_TIME_MS 1

void init_adc();

void do_capture();
