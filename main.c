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
#include <stdio.h>
#include <fft.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/

uint8_t ADC_bufferIndex = 0;

void ADC_init() {
     ADMUX  = 0b01100000; //Ref=Vcc, ADLARon
     ADMUX |= 0; //ADCx
     ADCSRA = 7;

	 ADCSRA |= _BV(ADEN);
}

void ADC_takeSapmples() {
	if(PIND & _BV(PD3)) {
		for(uint8_t i=0; i<128; ++i) {
			ADCSRA |= _BV(ADSC);
			while ( (ADCSRA & _BV(ADSC)) );
			FFT_ComplexBuffer[bitswap[i]].re = ADCH>>3;
			FFT_ComplexBuffer[bitswap[i]].im = 0;
		}		
	}else{
		for(uint8_t i=0; i<128; ++i) {
			ADCSRA |= _BV(ADSC);
			while ( (ADCSRA & _BV(ADSC)) );
			FFT_ComplexBuffer[i].re = ADCH>>3;
			FFT_ComplexBuffer[i].im = 0;
		}
	}
}

char text[64];

void plot(void) {
	for(uint8_t x=1; x<84; ++x) {
		LCD_line(x-1, 10+(FFT_ComplexBuffer[x-1].re), x, 10+(FFT_ComplexBuffer[x].re) );
	}
}

void stem(void) {
	for(uint8_t x=0; x<64; ++x) {
		LCD_line(x+5, LCDHEIGHT-1, 5+x, LCDHEIGHT-1-(FFT_ComplexBuffer[x].re>>4) );
	}
}

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/
int main(void) {
    uart_init(UART_BAUD_SELECT(115200, F_CPU));
	sei();
	LCD_init();
	ADC_init();
	DDRD &= ~(_BV(PD3));

	while(1) {

		// Print all
		LCD_clear();
		if(PIND & _BV(PD3)) {
			for(uint8_t i=0; i<128; ++i) {
				ADCSRA |= _BV(ADSC);
				while ( (ADCSRA & _BV(ADSC)) );
				FFT_ComplexBuffer[bitswap[i]].re = ADCH>>3;
				FFT_ComplexBuffer[bitswap[i]].im = 0;
			}
			FFT_calculate();
			stem();				
		}else{
			for(uint8_t i=0; i<128; ++i) {
				ADCSRA |= _BV(ADSC);
				while ( (ADCSRA & _BV(ADSC)) );
				FFT_ComplexBuffer[i].re = ADCH>>3;
				FFT_ComplexBuffer[i].im = 0;
			}
			plot();
		}
		LCD_showBuffer();			
	}

	while(1);

	//   AND THE ULTIMATE ANSWER IIIISSS !!!

	//  #   #  ####
	//	#   #     #
	//	#####  ####
	//	    #  #
	//	    #  ####

			// Rainbowdash is the best pony !

}
