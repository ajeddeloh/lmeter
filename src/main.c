#include "stm32l4xx_hal.h"

#include "uart.h"
#include "adc.h"
#include "clock.h"
#include "dac.h"
#include "sine.h"

#include <stdio.h>
#include <string.h>
#include <complex.h>

#define M_PI 3.14159265358979323846264338327f
#define RESISTOR_VALUE 1000.0f

static void init_gpio();
static float complex get_impedance(int16_t *data, size_t len, const Sine *sine);
void linear_regression(size_t n, const float *xs, const float complex *ys, float complex *res_m,
		float complex *res_b);

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
	float complex zs[N_SINES];
	float omegas[N_SINES];

	for (size_t i = 0; i < N_SINES; i++) {
		omegas[i] = 2.0f*M_PI*80e6f/(DAC_CYCLES_PER_UPDATE*SINES[i].len);
	}

	while(1) {
		for (size_t i = 0; i < N_SINES; i++) {
			const Sine *sine = &SINES[i];
			// Set the output frequency and wait for it to be steady
			change_sine(sine);
			HAL_Delay(100);
			// Toggle PB6 before and after so we can trigger on it with the
			// scope if needed
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
			// make sure we capture an interger number 
			data = do_capture(ADC_BUF_LEN/(sine->len) * sine->len);
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);

			zs[i] = get_impedance(data, ADC_BUF_LEN, sine);
			snprintf(print_buf, sizeof(print_buf), "%d %f %f\n",
					i, creal(zs[i]), cimag(zs[i]));
			HAL_USART_Transmit(&usart_handle, (uint8_t*)print_buf,
					strlen(print_buf), 1000);

		}
		float complex l, r;
		linear_regression(N_SINES, omegas, zs, &l, &r);
		snprintf(print_buf, sizeof(print_buf), "m=%f+%fi b=%f+%fi\n",
				creal(l), cimag(l), creal(r), cimag(r));
		HAL_USART_Transmit(&usart_handle, (uint8_t*)print_buf,
				strlen(print_buf), 1000);

	}
}

// Linear regression takes a list of xs and a list of ys (of size n) and does simple
// linear regression to determine the best fit line. It returns the slope in res_m and the
// y intercept in res_b.
void linear_regression(size_t n, const float *xs, const float complex *ys,
		float complex *res_m, float complex *res_b) {
	float complex numerator = 0;
	float complex denominator = 0;
	float complex mean_x = 0;
	float complex mean_y = 0;
	for (size_t i = 0; i < n; i++) {
		mean_x += xs[i];
		mean_y += ys[i];
	}
	mean_x /= n;
	mean_y /= n;
	for (size_t i = 0; i < n; i++) {
		numerator += (xs[i]-mean_x)*(ys[i]-mean_y);
		denominator += (xs[i]-mean_x)*(xs[i]-mean_x);
	}
	*res_m = numerator/denominator;
	*res_b = mean_y-(*res_m) * mean_x;
}

// get_impedance calculates the impendance of the inductor based on the ADC readings in data.
// The two ADC channels are interleaved in data. sine is the sine wave at the same frequency
// we are applying.
float complex get_impedance(int16_t *data, size_t len, const Sine *sine) {
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
	float complex impedance = (inductor/total)*RESISTOR_VALUE/(1-(inductor/total));
	return impedance;
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

