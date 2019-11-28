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
uint8_t ADC_bufferIndex = 0;
//uint8_t ADC_done = 0;

ISR(ADC_vect) {
	ADC_buffer[ADC_bufferIndex++] = ADCH;
	/*if(ADC_bufferIndex>=0x7F) {
		ADC_bufferIndex = 0;
		ADC_done = 1;
		cli();
	}*/
	//while(!(ADCSRA & _BV(ADIF)));
	/*if(ADC_bufferIndex>=129) {
		ADC_bufferIndex = 0;
		cli();
	}*/

	//ADCSRA &= ~_BV(ADIF);
	//ADCSRA |= _BV(ADSC); // Start
}

void ADC_init() {
     ADMUX  = 0b01100000;
     ADMUX |= 0; //ADCx
     ADCSRA = _BV(ADIE)|6;// 4 or 7
     ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADATE); //ADC enable, autotrigger

    //TIM_config_prescaler(TIM0, TIM_PRESC_8);
    //TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);
}

void ADC_takeSpamples() {
	//ADC_done = 0;
	sei();
	ADCSRA |= _BV(ADSC); // Start
	//while(!ADC_done);

    //ADCSRA |= _BV(ADSC); // Start
	//while(!ADC_bufferIndex);
	//ADC_buffer[0] = ADCH;
	//ADC_bufferIndex = 0;
	//_delay_ms(10);
	//sei();
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
	LCD_line(0, 0, 25, 25);
	LCD_showBuffer();

	ADC_init();
	while(1) {
		ADC_takeSpamples();
		LCD_clear();
		cli();
		plot();
		sei();
		LCD_showBuffer();
		_delay_ms(100);
	}

	//ADC_takeSpamples();

	//while(ADC_bufferIndex);


	while(1);

	//   AND THE ULTIMATE ANSWER IIIISSS !!!

	//  #   #  ####
	//	#   #     #
	//	#####  ####
	//	    #  #
	//	    #  ####

			// Rainbowdash is the best pony !

}
