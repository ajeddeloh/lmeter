#include "stm32l4xx_hal.h"

#include "uart.h"
#include "adc.h"
#include "clock.h"

static void init_gpio();

int main(void) {
	USART_HandleTypeDef usart_handle = {0};
	HAL_Init();
	init_clock();
	init_gpio();
	init_adc();
	init_usart(&usart_handle);
	do_capture();

	while(1) {
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		HAL_USART_Transmit(&usart_handle, (uint8_t*)"hello",
				sizeof("hello"), 1000);
	}
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

