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

static void init_gpio();

int main(void) {
	size_t lens[32];
	size_t n_lens = sizeof(lens)/sizeof(lens[0]);
	for (size_t i = 0; i < n_lens; i++) {
		lens[i] = 512-8*i;
	}
	HAL_Init();
	init_clock();
	USART_HandleTypeDef usart_handle = {0};
	init_usart(&usart_handle);
	init_gpio();
	init_adc();

	Sine *sine = get_sine(512);
	init_dac(sine);

	char print_buf[128];
	volatile int16_t *data;
	float complex zs[n_lens];
	float omegas[n_lens];
	for (size_t i = 0; i < n_lens; i++) {
		omegas[i] = 2.0f*M_PI*80e6f/(DAC_CYCLES_PER_UPDATE*lens[i]);
	}

	while(1) {
		for (size_t i = 0; i < n_lens; i++) {
			const Sine *sine = get_sine(lens[i]);
			size_t capture_len = ADC_BUF_LEN/(sine->len) * sine->len;
			// Set the output frequency and wait for it to be steady
			dac_change_sine(sine);
			HAL_Delay(20);
			// Toggle PB6 before and after so we can trigger on it with the
			// scope if needed
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
			// make sure we capture an interger number 
			data = do_capture(capture_len);
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);

			zs[i] = get_impedance(data, capture_len, sine);

		}
		float complex l, r;
		linear_regression(n_lens, omegas, zs, &l, &r);
		snprintf(print_buf, sizeof(print_buf), "m=%e+%ei b=%e+%ei\n",
				creal(l), cimag(l), creal(r), cimag(r));
		HAL_USART_Transmit(&usart_handle, (uint8_t*)print_buf,
				strlen(print_buf), 1000);

	}
}

void init_gpio() {
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_2 | GPIO_PIN_6,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Pull = GPIO_NOPULL,
	};
	HAL_GPIO_Init(GPIOB, &config);	
}

