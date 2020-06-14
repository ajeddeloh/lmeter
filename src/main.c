#include "stm32l4xx_hal.h"

#define TX_PIN GPIO_PIN_5
#define RX_PIN GPIO_PIN_6

static void init_gpio();
static void init_usart(USART_HandleTypeDef *h);

int main(void) {
	USART_HandleTypeDef usart_handle = {0};
	HAL_Init();
	init_gpio();
	init_usart(&usart_handle);
	
	while(1) {
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		HAL_USART_Transmit(&usart_handle, (uint8_t*)"hello",
				sizeof("hello"), 1000);
	}
}

void init_usart(USART_HandleTypeDef *h) {
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
