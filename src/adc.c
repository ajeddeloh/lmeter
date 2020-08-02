#include <complex.h>

#include "stm32l4xx_hal.h"

#include "sine.h"
#include "adc.h"
#include "bit_band.h"

#define RESISTOR_VALUE 1000.0f

// *2 because we have 2 channels, / 4 because the DAC updates 4x per ADC sample
static volatile int16_t data[MAX_SINE_LEN * 2 / 4];
static volatile size_t n_half_waves = 0;
static volatile int64_t ind_real_sum, ind_imag_sum, tot_real_sum, tot_imag_sum;
static const Sine *adc_sine = NULL;

void adc_init() {
	// Enable Interrupts
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	// Init the clocks for ADC, GPIOA, DMA1
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN | RCC_AHB2ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	// Set ADC clock to be in sync with sysclock, turn on circular DMA and dual ADCs
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0 | ADC_CCR_MDMA_1
		| ADC_CCR_DUAL_0 | ADC_CCR_DMACFG;
	
	// Init the GPIO (PA0-PA3)
	// Should be in analog mode after reset already
	GPIOA->ASCR = GPIO_ASCR_ASC0 | GPIO_ASCR_ASC1
		| GPIO_ASCR_ASC2 | GPIO_ASCR_ASC3;

	// Init the DMA
	// ADC1, Chan 1 defaults to ADC1
	DMA1_Channel1->CPAR = (uint32_t)&ADC123_COMMON->CDR;
	DMA1_Channel1->CMAR = (uint32_t)data;
	DMA1_Channel1->CCR |= DMA_CCR_PL_0 | DMA_CCR_PL_1  // highest priority
		| DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 // move 32 bit words
		| DMA_CCR_MINC | DMA_CCR_CIRC // increment the memory addr in circular mode
		| DMA_CCR_TEIE | DMA_CCR_HTIE | DMA_CCR_TCIE; // enable interrupts

	// Init the adc
	// turn off deep power down DEEPPWD=0 and turn on the voltage regulator. 
	ADC1->CR = ADC_CR_ADVREGEN;
	ADC2->CR = ADC_CR_ADVREGEN;
	// wait setup time (at least 20us)
	HAL_Delay(ADC_SETUP_TIME_MS);

	ADC1->SMPR1 |= ADC_SMPR1_SMP5_2; // 47.5 cycles sampling + 12.5 conv = 60
	ADC2->SMPR1 |= ADC_SMPR1_SMP7_2; // 80e6/60 = 1.3333 Msps
	ADC1->SQR1 = (CHAN1 << ADC_SQR1_SQ1_Pos); // select the channels for the gpio
	ADC2->SQR1 = (CHAN2 << ADC_SQR1_SQ1_Pos);
	ADC1->DIFSEL = 1 << CHAN1; // turn on differential mode
	ADC2->DIFSEL = 1 << CHAN2;
	// set the offset to 2048 so our data is centered around 0
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
	// turn on continous mode, only need to write to ADC1 since we're in dual mode
	ADC1->CFGR |= ADC_CFGR_CONT;
}

// adc_capture captures n_waves number of full cycles of the current_sine sine wave and
// returns the impedance calculated from the data
complex float adc_capture(size_t n_waves, const Sine *current_sine) {
	adc_sine = current_sine;
	n_half_waves = n_waves*2;
	ind_real_sum = 0;
	ind_imag_sum = 0;
	tot_real_sum = 0;
	tot_imag_sum = 0;

	// the DAC updates 4x faster than the ADC, so we only need 1/4 the samples
	DMA1_Channel1->CNDTR = adc_sine->len/4;
	// turn on DMA and start the ADC
	BB(DMA1_Channel1->CCR)[DMA_CCR_EN_Pos] = 1;
	ADC1->CR |= ADC_CR_ADSTART;

	while(n_half_waves != 0);

        float complex inductor = ind_real_sum + I * ind_imag_sum;
        float complex total = tot_real_sum + I * tot_imag_sum;
        float complex impedance = (inductor/total)*RESISTOR_VALUE/(1-(inductor/total));
        return impedance;
}

/*
 * Process the adc data, accumulating into the {ind,tot}_{real,imag}_sum variables.
 * The ADC is running at 60 cycles/sample, so this needs to process them faster than that.
 */
void DMA1_Channel1_IRQHandler() {
	// toggle the GPIO pin so I can look at timing on the scope
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	if (n_half_waves == 1) {
		// last half wave, turn off the adc and stop the DMA
		ADC1->CR |= ADC_CR_ADSTP;
		BB(DMA1_Channel1->CCR)[DMA_CCR_EN_Pos] = 0;
	}

	const size_t len = adc_sine->len / 4;
	size_t offset = 0;
	if (BB(DMA1->ISR)[DMA_ISR_TCIF1_Pos]) {
		// Transfer complete means we already processed the first half of the
		// buffer (when we got the half complete irq) and we should process the
		// second half.
		offset = len / 2;
	}

	// Non-volatile versions of the static variables. The ISR is the only reason
	// the static ones are volatile, so they don't need to be volatile here. In fact
	// having them be volatile severely hurts performance to the point were the ISR
	// does not finish in time for the next ISR.
	int64_t i_real_sum = 0;
	int64_t i_imag_sum = 0;
	int64_t t_real_sum = 0;
	int64_t t_imag_sum = 0;

	for (size_t i = 0; i < len / 2; i++) {
		size_t idx = i + offset;
                int32_t tot = data[idx*2];
                int32_t ind = data[idx*2 + 1];
                int32_t sin = adc_sine->sin_high_res[idx];
                int32_t cos = adc_sine->cos_high_res[idx];
                i_real_sum += ind*cos;
                i_imag_sum -= ind*sin;
                t_real_sum += tot*cos;
                t_imag_sum -= tot*sin;
	}
	// update the volatile counterparts
	ind_real_sum += i_real_sum;
	ind_imag_sum += i_imag_sum;
	tot_real_sum += t_real_sum;
	tot_imag_sum += t_imag_sum;
	n_half_waves--;
	DMA1->IFCR = 0xf;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}
