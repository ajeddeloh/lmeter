#include "stm32l4xx_hal.h"

void SysTick_Handler() {
	HAL_IncTick();
}
