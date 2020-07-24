#include "stm32l4xx_hal.h"

#include "uart.h"
#include "adc.h"
#include "clock.h"
#include "dac.h"
#include "sine.h"
#include "calc.h"

#include <stdio.h>
#include <string.h>
#include <complex.h>

#define M_PI 3.14159265358979323846264338327f
#define N_FREQS 32

static void init_gpio();
static void get_inductance(const size_t *lens, const float *omegas, float complex *l, float complex *r);

int main(void) {
	HAL_Init();
	init_clock();
	USART_HandleTypeDef usart_handle = {0};
	init_usart(&usart_handle);
	init_gpio();
	init_adc();
	init_dac();

	char print_buf[128];
	size_t lens[N_FREQS];
	float omegas[N_FREQS];
	for (size_t i = 0; i < N_FREQS; i++) {
		lens[i] = 512-8*i; // [512, 256) in steps of 8
		omegas[i] = 2.0f*M_PI*80e6f/(DAC_CYCLES_PER_UPDATE*lens[i]);
	}

	while(1) {
		float complex l, r;
		get_inductance(lens, omegas, &l, &r);
		snprintf(print_buf, sizeof(print_buf), "m=%e+%ei b=%e+%ei\n",
				creal(l), cimag(l), creal(r), cimag(r));
		HAL_USART_Transmit(&usart_handle, (uint8_t*)print_buf,
				strlen(print_buf), 1000);
	}
}

static void get_inductance(const size_t *lens, const float *omegas, float complex *l, float complex *r) {
	float complex zs[N_FREQS];
	for (size_t i = 0; i < N_FREQS; i++) {
		const Sine *sine = get_sine(lens[i]);
		// make sure we capture an interger number of waves
		size_t capture_len = ADC_BUF_LEN/(sine->len) * sine->len;
		// Set the output frequency and wait for it to be steady
		dac_change_sine(sine);
		HAL_Delay(20); // 20ms is arbitrary, seems to work fine
		// Toggle PB6 before and after so we can trigger on it with the scope if needed

		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
		volatile int16_t *data = do_capture(capture_len);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);

		zs[i] = get_impedance(data, capture_len, sine);
	}
	linear_regression(N_FREQS, omegas, zs, l, r);
}

static void init_gpio() {
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_2 | GPIO_PIN_6,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Pull = GPIO_NOPULL,
	};
	HAL_GPIO_Init(GPIOB, &config);	
}
