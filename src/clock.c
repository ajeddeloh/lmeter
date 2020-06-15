#include "stm32l4xx_hal.h"
#include "clock.h"

void init_clock() {
	// Set the clock to the HSE clock (8MHz) with a PLL of x10
	// for a final freq of 80MHz
	RCC_OscInitTypeDef osc = {
		.OscillatorType = RCC_OSCILLATORTYPE_HSE,
		.HSEState = RCC_HSE_ON,
		.PLL = {
			.PLLState = RCC_PLL_ON,
			.PLLSource = RCC_PLLSOURCE_HSE,
			.PLLM = 1,
			.PLLN = 20, //160MHz
			.PLLP = 7,  //22MHz ish
			.PLLQ = 4,  //40Mhz
			.PLLR = 2,  //80MHz
		},
	};
	if (HAL_RCC_OscConfig(&osc) != HAL_OK) {
		while(1);
	}

	RCC_ClkInitTypeDef clk = {
		.ClockType = RCC_CLOCKTYPE_SYSCLK,
		.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
		.AHBCLKDivider = RCC_SYSCLK_DIV1,
		.APB1CLKDivider = RCC_SYSCLK_DIV1,
		.APB2CLKDivider = RCC_SYSCLK_DIV1,
	};

	if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_4) != HAL_OK) {
		while(1);
	}
}
