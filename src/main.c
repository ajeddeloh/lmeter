#include "stm32l4xx_hal.h"

#include "uart.h"
#include "adc.h"
#include "clock.h"
#include "dac.h"
#include "sine.h"

#include <stdio.h>
#include <string.h>
#include <complex.h>

static void init_gpio();
static float get_inductance(int16_t *data, size_t len);

int main(void) {
	USART_HandleTypeDef usart_handle = {0};
	HAL_Init();
	init_clock();
	init_gpio();
	init_adc();
	init_usart(&usart_handle);
	init_dac();

	while(1) {
		int16_t *data = do_capture();
		float z;
		z = get_inductance(data, ADC_BUF_LEN);
		char buf[20];
		sprintf(buf, "%f\n", z);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		HAL_USART_Transmit(&usart_handle, (uint8_t*)buf,
				strlen(buf), 1000);
	}
}

float get_inductance(int16_t *data, size_t len) {
	int64_t i_real = 0;
	int64_t i_imag = 0;
	int64_t t_real = 0;
	int64_t t_imag = 0;
	for (size_t i = 0; i < len; i+=2) {
		int64_t tot = data[i];
		int64_t ind = data[i + 1];
		int64_t sin = sine[(i/2) % n_sine] - 2048;
		int64_t cos = sine[(i/2 + 3*n_sine/4) % n_sine] - 2048;
		i_real += ind*cos;
		i_imag -= ind*sin;
		t_real += tot*cos;
		t_imag -= tot*sin;
	}
	float complex inductor = i_real + I * i_imag;
	float complex total = t_real + I * t_imag;
	float complex impedance = (inductor/total)*1000/(1-inductor/total);
	return cimag(impedance);
}

void init_gpio() {
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_2,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Pull = GPIO_NOPULL,
	};
	HAL_GPIO_Init(GPIOB, &config);	
}

