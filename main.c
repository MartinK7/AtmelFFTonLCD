/*
 * ---------------------------------------------------------------------
 * Author:      Martin Krásl & Marek Raška
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     
 * Last update: 
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: 
 * TODO:
 */

/* Includes ----------------------------------------------------------*/
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <crazypinout.h>
#include <nokialcd.h>
#include <timer.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/

uint8_t ADC_buffer[128];
uint8_t ADC_bufferIndex;

ISR(TIMER0_OVF_vect) {
	//ADCSRA |= ADCSRA;
	ADC_buffer[ADC_bufferIndex++] = ADCH;
	ADCSRA |= ADSC;
	if(!ADC_bufferIndex)
		cli();
}

void ADC_init() {
     ADMUX  = 0b01000000;
     ADMUX |= 0; //ADCx
     ADCSRA = _BV(ADIE)|1;
     ADCSRA |= _BV(ADEN); //ADC enable

    //TIM_config_prescaler(TIM0, TIM_PRESC_8);
    //TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);
}

void ADC_takeSpamples() {
/*
	ADC_bufferIndex = 1;
	_delay_ms(10);
	//sei();
	while(!ADC_bufferIndex);*/
}

void plot(void) {
	for(uint8_t x=1; x<84; ++x) {
		LCD_line(x-1, 10+(ADC_buffer[x-1]>>3), x, 10+(ADC_buffer[x]>>3) );
	}
}

void stem(void) {
	for(uint8_t x=0; x<84; ++x) {
		LCD_line(x, LCDHEIGHT-1, x, LCDHEIGHT-1-(ADC_buffer[x]>>3) );
	}
}

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/
int main(void) {
	LCD_init();

	LCD_clear();
	LCD_line(0, 0, 50, 50);
	LCD_showBuffer();

	ADC_init();
	ADC_takeSpamples();

	LCD_clear();
	plot();
	LCD_showBuffer();

	while(1);

	//   AND THE ULTIMATE ANSWER IIIISSS !!!

	//  #   #  ####
	//	#   #     #
	//	#####  ####
	//	    #  #
	//	    #  ####

			// Rainbowdash is the best pony !

}
