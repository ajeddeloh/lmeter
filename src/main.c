#include "stm32l4xx_hal.h"

#include "uart.h"
#include "adc.h"
#include "clock.h"
#include "dac.h"
#include "sine.h"

#include <stdio.h>
#include <string.h>
#include <complex.h>

#define M_PI 3.14159265358979323846264338327

static void init_gpio();
static float complex get_inductance(int16_t *data, size_t len, const Sine *sine);

int main(void) {
	HAL_Init();
	init_clock();
	USART_HandleTypeDef usart_handle = {0};
	init_usart(&usart_handle);
	init_gpio();
	init_adc();
	init_dac(&SINES[0]);

	char print_buf[128];
	int16_t *data;
	float complex z;

	while(1) {
		// Toggle PB6 before and after so we can trigger on it with the scope if
		// needed
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
		data = do_capture();
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);

		z = get_inductance(data, ADC_BUF_LEN, &SINES[0]);
		snprintf(print_buf, sizeof(print_buf), "%f %f\n", creal(z), cimag(z));
		HAL_USART_Transmit(&usart_handle, (uint8_t*)print_buf,
				strlen(print_buf), 1000);

		HAL_Delay(1000);
	}
}

float complex get_inductance(int16_t *data, size_t len, const Sine *sine) {
	// Compute sum( f(t) * e^(i*omega) ) for the the adc data at the frequency we're
	// measuring. Use int64_t since we could overflow 32 bits. Convert to float for the
	// math later, but keep them as ints while adding to preserve precision
	// TODO: use a seperate set of more precise sine tables for this rather than the dac
	// ones.
	int64_t i_real = 0;
	int64_t i_imag = 0;
	int64_t t_real = 0;
	int64_t t_imag = 0;
	for (size_t i = 0; i < len; i+=2) {
		int64_t tot = data[i];
		int64_t ind = data[i + 1];
		int64_t sin = sine->data[(i*2) % sine->len] - 2048;
		int64_t cos = sine->data[(i*2 + sine->len/4) % sine->len] - 2048;
		i_real += ind*cos;
		i_imag -= ind*sin;
		t_real += tot*cos;
		t_imag -= tot*sin;
	}
	float complex inductor = i_real + I * i_imag;
	float complex total = t_real + I * t_imag;
	float complex impedance = (inductor/total)*1000/(1-(inductor/total));
	float w = 2.0*M_PI*80e6/(DAC_CYCLES_PER_UPDATE*sine->len);
	return cimag(impedance/(w)) + I*creal(impedance);
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

