#include "stm32l4xx_hal.h"

#include "sine.h"
#include "dac.h"

void init_dac() {
	NVIC_EnableIRQ(DMA2_Channel5_IRQn);

	// Init the clocks for DAC, GPIOx, DMA2, TIM6
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN | RCC_APB1ENR1_TIM6EN;
	
	// No GPIO init needed

	// Init the DMA
	// ADC1, Chan 1 defaults to ADC1
	DMA2_Channel5->CNDTR = 512;
	DMA2_Channel5->CMAR = (uint32_t)sine512;
	DMA2_Channel5->CPAR = (uint32_t)&DAC1->DHR12R2;
	DMA2_CSELR->CSELR = 3 << DMA_CSELR_C5S_Pos;
	DMA2_Channel5->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1 | DMA_CCR_MSIZE_0
		| DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_EN | DMA_CCR_TEIE
		| DMA_CCR_CIRC | DMA_CCR_DIR;
	
	// Init the DAC
	DAC->CR |= DAC_CR_EN2 | DAC_CR_TEN2 | DAC_CR_DMAEN2;

	// Init and start the timer
	TIM6->ARR = 14; // update four times every ADC cycle
	TIM6->CR2 |= TIM_CR2_MMS_1;
	TIM6->CR1 |= TIM_CR1_CEN;
}
