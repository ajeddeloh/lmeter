#include <string.h>

#include "stm32l4xx_hal.h"

#include "sine.h"
#include "dac.h"
#include "bit_band.h"

static int16_t dac_data[MAX_SINE_LEN];

void dac_init() {
	NVIC_EnableIRQ(DMA2_Channel5_IRQn);

	// Init the clocks for DAC, GPIOx, DMA2, TIM6
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN | RCC_APB1ENR1_TIM6EN;
	
	// No GPIO init needed

	// Init the DMA, except do not set up the source/len yet and leave it disabled
	// ADC1, Chan 1 defaults to ADC1
	DMA2_Channel5->CPAR = (uint32_t)&DAC1->DHR12R2;
	DMA2_Channel5->CMAR = (uint32_t)&dac_data;
	DMA2_CSELR->CSELR = 3 << DMA_CSELR_C5S_Pos;
	DMA2_Channel5->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1 | DMA_CCR_MSIZE_0
		| DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_TEIE
		| DMA_CCR_CIRC | DMA_CCR_DIR;
	
	// Init the DAC
	DAC->CR |= DAC_CR_EN2 | DAC_CR_TEN2 | DAC_CR_DMAEN2;

	// Init but do not start the timer
	TIM6->ARR = DAC_CYCLES_PER_UPDATE - 1; // update four times every ADC cycle
	TIM6->CR2 |= TIM_CR2_MMS_1;
}

void dac_change_sine(const Sine *sine) {
	// if we haven't started the sine yet just load the data and start
	if (BB(TIM6->CR1)[TIM_CR1_CEN_Pos] == 0) {
		memcpy(dac_data, sine->data, sine->len * 2);
		DMA2_Channel5->CNDTR = sine->len;
		BB(DMA2_Channel5->CCR)[DMA_CCR_EN_Pos] = 1;
		BB(TIM6->CR1)[TIM_CR1_CEN_Pos] = 1;
		return;
	}
	/* Changing the running sine involves:
	 *  - waiting until we're halfway done with the current sine wave by polling the half
	 * 	transfer complete flag on the DMA channel
	 *  - copying the new sine wave up to that point, since we know it wont be accessed
	 *  - waiting until the full transfer is complete by polling the transfer complete
	 *	flag in the DMA channel
	 *  - changing the DMA length between the last sample and the first sample
	 *  - copying the latter half of the new sine wave
	 *
	 *  Changing the length must be done in the 15 cycle window between samples because
	 *  it requires turning off DMA. Interrupts cannot be used since the best case latency
	 *  is 12 cycles before entering the ISR. Polling has lower latency.
	 */

	const size_t first_len = DMA2_Channel5->CNDTR / 2;

	// clear the flags for channel 5
	DMA2->IFCR = DMA_IFCR_CTCIF5 | DMA_IFCR_CHTIF5 | DMA_IFCR_CTEIF5 | DMA_IFCR_CGIF5;
	// wait for the first half to complete
	while (!BB(DMA2->ISR)[DMA_ISR_HTIF5_Pos]);
	// clear the flags again in case the tc flag got set
	DMA2->IFCR = DMA_IFCR_CTCIF5 | DMA_IFCR_CHTIF5 | DMA_IFCR_CTEIF5 | DMA_IFCR_CGIF5;
	// copy the first half
	memcpy(dac_data, sine->data, first_len*sizeof(dac_data[0]));

	// must be done in <15 cycles from the time DMA2_5 transfer completes 
	__asm volatile(
	// wait for the DAC to read the last element
	"loop%=:\n" // wait for the TC flag
	"	ldr	r0, [%[rTCIF]]\n"
	"	cmp	r0, #1\n"
	"	bne	loop%=\n"
	// turn off DMA, Use %[DMAen] as a 0 since only the LSB matter when bit-banding
	// and since all BB'd addressed are word aligned it's always 0;
	"	str	%[rDMAen], [%[rDMAen]]\n"
	"	str	%[rLen], [%[rDMAlen]]\n"
	"	str	r0, [%[rDMAen]]\n"
		: // no outputs, so the volatile keyword is redundant
		: [rTCIF]"l"(&BB(DMA2->ISR)[DMA_ISR_TCIF5_Pos]), // tcif5 flag bit
			[rDMAen]"l"(&BB(DMA2_Channel5->CCR)[DMA_CCR_EN_Pos]), // dma en bit
			[rLen] "l" (sine->len), // new len to load
			[rDMAlen] "l" (&DMA2_Channel5->CNDTR) // address to but new len in
		: "cc", "r0"
	);
	// end time critical area
	// transfer the second half
	memcpy(dac_data + first_len, sine->data + first_len,
			sizeof(dac_data[0])*(sine->len-first_len));
}
