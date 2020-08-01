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
#define N_WAVES_TO_CAPTURE 32

static void gpio_init();
static void get_inductance(const size_t *lens, const float *omegas, float complex *l, float complex *r);

int main(void) {
	HAL_Init();
	clock_init();
	USART_HandleTypeDef usart_handle = {0};
	usart_init(&usart_handle);
	gpio_init();
	adc_init();
	dac_init();

	char print_buf[128];
	size_t lens[N_FREQS*2]; // array of lengths of the sines
	float omegas[N_FREQS*2]; // array of omegas (2*pi*f)

	for (size_t i = 0; i < N_FREQS; i++) {
		lens[i] = 512-8*i; // [512, 256) in steps of 8
		lens[N_FREQS*2-1-i] = 512-8*i; // [512, 256) in steps of 8
		omegas[i] = 2.0f*M_PI*80e6f/(DAC_CYCLES_PER_UPDATE*lens[i]);
		omegas[N_FREQS*2-1-i] = 2.0f*M_PI*80e6f/(DAC_CYCLES_PER_UPDATE*lens[i]);
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
	float complex zs[N_FREQS*2];
	for (size_t i = 0; i < N_FREQS*2; i++) {
		const Sine *sine = get_sine(lens[i]);
		dac_change_sine(sine);
		HAL_Delay(3); // 3ms is arbitrary, seems to work fine

		zs[i] = adc_capture(N_WAVES_TO_CAPTURE, sine);
	}
	linear_regression(N_FREQS, omegas, zs, l, r);
}

static void gpio_init() {
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_2 | GPIO_PIN_6,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Pull = GPIO_NOPULL,
	};
	HAL_GPIO_Init(GPIOB, &config);	
}
