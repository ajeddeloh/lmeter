#pragma once

#include "stm32l4xx_hal.h"

#define TX_PIN GPIO_PIN_5
#define RX_PIN GPIO_PIN_6

void init_usart(USART_HandleTypeDef *h);
