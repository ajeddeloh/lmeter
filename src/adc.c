#include "stm32l4xx_hal.h"

#include "adc.h"

static int16_t data[ADC_BUF_LEN*2];

void init_adc() {
	// Enable Interrupts
	NVIC_EnableIRQ(ADC1_2_IRQn);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	// Init the clocks for ADC, GPIOA, DMA1
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN | RCC_AHB2ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0 | ADC_CCR_MDMA_1 | ADC_CCR_DUAL_0;
	
	// Init the GPIO (PA0-PA3)
	// Should be in analog mode after reset already
	GPIOA->ASCR = GPIO_ASCR_ASC0 | GPIO_ASCR_ASC1
		| GPIO_ASCR_ASC2 | GPIO_ASCR_ASC3;

	// Init the DMA
	// ADC1, Chan 1 defaults to ADC1
	DMA1_Channel1->CNDTR = ADC_BUF_LEN;
	DMA1_Channel1->CPAR = (uint32_t)&ADC123_COMMON->CDR;
	DMA1_Channel1->CMAR = (uint32_t)data;
	DMA1_Channel1->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1 | DMA_CCR_MSIZE_1
		| DMA_CCR_PSIZE_1 | DMA_CCR_MINC | DMA_CCR_EN | DMA_CCR_TEIE;

	// Init the adc
	// turn off deep power down DEEPPWD=0 and turn on the voltage
	// regulator. 
	ADC1->CR = ADC_CR_ADVREGEN;
	ADC2->CR = ADC_CR_ADVREGEN;
	// wait setup time
	HAL_Delay(ADC_SETUP_TIME_MS);

	ADC1->SMPR1 |= ADC_SMPR1_SMP5_2; // 47.5 cycles sampling + 12.5 conv = 60
	ADC2->SMPR1 |= ADC_SMPR1_SMP7_2; // 80e6/60 = 1.3333 Msps
	ADC1->SQR1 = (CHAN1 << ADC_SQR1_SQ1_Pos);
	ADC2->SQR1 = (CHAN2 << ADC_SQR1_SQ1_Pos);
	ADC1->DIFSEL = 1 << CHAN1;
	ADC2->DIFSEL = 1 << CHAN2;
	ADC1->OFR1 = ADC_OFR1_OFFSET1_EN | (CHAN1 << ADC_OFR1_OFFSET1_CH_Pos) | 2048;
	ADC2->OFR1 = ADC_OFR1_OFFSET1_EN | (CHAN2 << ADC_OFR1_OFFSET1_CH_Pos) | 2048;

	// Calibrate
	// Set ADCAL=1
	ADC1->CR |= ADC_CR_ADCAL | ADC_CR_ADCALDIF;
	ADC2->CR |= ADC_CR_ADCAL | ADC_CR_ADCALDIF;
	// poll for calibration to end
	while ((ADC1->CR | ADC2->CR) & ADC_CR_ADCAL_Msk);
	
	// Enable the ADC and wait for it to be ready
	ADC1->ISR &= ~ADC_ISR_ADRDY_Msk;
	ADC2->ISR &= ~ADC_ISR_ADRDY_Msk;
	ADC1->CR |= ADC_CR_ADEN;
	ADC2->CR |= ADC_CR_ADEN;
	while (!(ADC1->ISR & ADC2->ISR & ADC_ISR_ADRDY_Msk));
	ADC1->CFGR |= ADC_CFGR_CONT;
}

int16_t *do_capture() {
	// flip the DMA on and off again so we can reload the counter
	DMA1_Channel1->CCR &= ~DMA_CCR_EN;
	DMA1_Channel1->CNDTR = ADC_BUF_LEN;
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	ADC1->CR |= ADC_CR_ADSTART;
	while(DMA1_Channel1->CNDTR != 0);
	ADC1->CR |= ADC_CR_ADSTP;
	return data;
}

void ADC1_2_IRQHandler() {}
