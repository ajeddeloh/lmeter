#include "stm32l4xx_hal.h"

#include "adc.h"

#define CHAN1 6

static uint16_t data[ADC_BUF_LEN];

void init_adc() {

	NVIC_EnableIRQ(ADC1_2_IRQn);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	// Init the clock
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN | RCC_AHB2ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE;
	
	// Init the GPIO (PA1, PA2)
	// Would be good to use any of the PC0-3 pins since they're on fast
	// ADC channels, but the Disco board uses them for other things instead.
	// Should be in analog mode after reset already
	GPIOA->ASCR = GPIO_ASCR_ASC1 | GPIO_ASCR_ASC2;

	// Init the DMA
	// ADC1, Chan 1 defaults to ADC1
	DMA1_Channel1->CNDTR = ADC_BUF_LEN;
	DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
	DMA1_Channel1->CMAR = (uint32_t)data;
	DMA1_Channel1->CCR |= (3 << DMA_CCR_PL_Pos) | (1 << DMA_CCR_MSIZE_Pos)
		| (1 << DMA_CCR_PSIZE_Pos) | DMA_CCR_MINC | DMA_CCR_EN | DMA_CCR_TEIE;

	// Init the adc
	// turn off deep power down DEEPPWD=0 and turn on the voltage
	// regulator. 
	ADC1->CR = ADC_CR_ADVREGEN;
	// wait setup time
	HAL_Delay(ADC_SETUP_TIME_MS);
	ADC1->SMPR1 = 1; // 6.5cycles for channel 1
	ADC1->SQR1 = (CHAN1 << ADC_SQR1_SQ1_Pos);
	ADC1->DIFSEL = ADC_DIFSEL_DIFSEL_6;
	ADC1->OFR1 = ADC_OFR1_OFFSET1_EN | (CHAN1 << ADC_OFR1_OFFSET1_CH_Pos) | 2048;

	// Calibrate
	// Set ADCAL=1
	ADC1->CR |= ADC_CR_ADCAL | ADC_CR_ADCALDIF;
	// fuck it, keep polling for adcal=0
	while (ADC1->CR & ADC_CR_ADCAL_Msk);
	
	// Enable the ADC and wait for it to be ready
	ADC1->ISR &= ~ADC_ISR_ADRDY_Msk;
	ADC1->CR |= ADC_CR_ADEN;
	while (!(ADC1->ISR & ADC_ISR_ADRDY_Msk));
	ADC1->CFGR |= ADC_CFGR_CONT | ADC_CFGR_DMAEN;
}

void do_capture() {
	ADC1->CR |= ADC_CR_ADSTART;
	while(DMA1_Channel1->CNDTR != 0);
	ADC1->CR |= ADC_CR_ADSTP;
}

void ADC1_2_IRQHandler() {}
