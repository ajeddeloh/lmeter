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

#define N_WAVES_TO_CAPTURE 512

static void gpio_init();
static void get_inductance(float complex *l, float complex *r);

int main(void) {
	HAL_Init();
	clock_init();
	USART_HandleTypeDef usart_handle = {0};
	usart_init(&usart_handle);
	gpio_init();
	adc_init();
	dac_init();
	sine_populate();

	char print_buf[128];

	while(1) {
		float complex l, r;
		get_inductance(&l, &r);
		snprintf(print_buf, sizeof(print_buf), "m=%e+%ei b=%e+%ei\n",
				creal(l), cimag(l), creal(r), cimag(r));
		HAL_USART_Transmit(&usart_handle, (uint8_t*)print_buf,
				strlen(print_buf), 1000);
	}
}

// get_inductance returns the inductance in l and series resistance in r
static void get_inductance(float complex *l, float complex *r) {
	float complex zs[N_WAVES*2];
	float omegas[N_WAVES*2];
	for (int sine_idx = 0; sine_idx < N_WAVES; sine_idx++) {
		const Sine *sine = get_sine(sine_idx);
		dac_change_sine(sine);
		HAL_Delay(3); // 3ms is arbitrary, seems to work fine

		omegas[sine_idx] = sine->omega;
		zs[sine_idx] = adc_capture(N_WAVES_TO_CAPTURE, sine);
	}
	for (int i = N_WAVES; i < N_WAVES*2; i++) {
		int sine_idx = N_WAVES*2 - 1 - i;
		const Sine *sine = get_sine(sine_idx);
		dac_change_sine(sine);
		HAL_Delay(3); // 3ms is arbitrary, seems to work fine

		omegas[i] = sine->omega;
		zs[i] = adc_capture(N_WAVES_TO_CAPTURE, sine);
	}
	linear_regression(N_WAVES, omegas, zs, l, r);
}

static void gpio_init() {
	// this is mostly so we can toggle GPIO pins for triggering on a scope
	// or timing critical sections
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_2 | GPIO_PIN_6,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Pull = GPIO_NOPULL,
	};
	HAL_GPIO_Init(GPIOB, &config);	
}
