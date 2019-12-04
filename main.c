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
#include <uart.h>
#include <fft.h>
#include <stdio.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/

uint8_t ADC_bufferIndex = 0;
//uint8_t ADC_done = 0;

ISR(ADC_vect) {
	if(PIND & _BV(PD3))
		FFT_pushDataSwap(ADCH>>3, ADC_bufferIndex++);
	else
		FFT_pushData(ADCH>>3, ADC_bufferIndex++);
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
     ADCSRA = _BV(ADLAR) | 6;// 4 or 7
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

char text[64];

void plot(void) {
	TCplx *dat = FFT_getPtrData();
	for(uint8_t x=1; x<64; ++x) {
		LCD_line(x-1, 10+(dat[x-1].re), x, 10+(dat[x].re) );
	}
}

void stem(void) {
	TCplx *dat = FFT_getPtrData();
	int16_t val;
	uint16_t valu;
	for(uint8_t x=0; x<64; ++x) {
		val = dat[x].re;
		valu = (uint16_t)val;
		valu = valu>>5;
		LCD_line(x+5, LCDHEIGHT-1, 5+x, LCDHEIGHT-1-(valu) );
	}
}

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/
int main(void) {
	LCD_init();
	ADC_init();
    uart_init(UART_BAUD_SELECT(115200, F_CPU));

	DDRD &= ~(_BV(PD3));

	sei();

	while(1) {
		ADC_takeSpamples();

		// Print all
		LCD_clear();
		if(PIND & _BV(PD3)) {
			uart_puts("FFT START!!!!!\r\n");
			FFT_calculate();
			uart_puts("FFT END!!!!!\r\n");

			stem();
		}else{
			plot();
		}
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
