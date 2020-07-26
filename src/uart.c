#include "stm32l4xx_hal.h"
#include "uart.h"

void usart_init(USART_HandleTypeDef *h) {
	// USART2 is connected to the uart<->usb controller
	h->Instance = USART2;
	USART_InitTypeDef config ={
		.BaudRate = 9600,
		.WordLength = USART_WORDLENGTH_8B,
		.StopBits = USART_STOPBITS_1,
		.Parity = USART_PARITY_NONE,
		.Mode = USART_MODE_TX_RX,
		//.HwFlowCtl = UART_HWCONTROL_NONE,
	};
	h->Init = config;
	if (HAL_USART_Init(h) != HAL_OK) {
		while(1);
	}
}

void HAL_USART_MspInit(USART_HandleTypeDef *usart) {
	(void) usart; // Shut up, compiler
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_config = {
		.Mode = GPIO_MODE_AF_PP,
		.Pin = TX_PIN,
		.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
		.Pull = GPIO_PULLUP,
		.Alternate = GPIO_AF7_USART2,
	};
	HAL_GPIO_Init(GPIOD, &gpio_config);
}
