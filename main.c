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

ISR(ADC_vect) {
	if(PIND & _BV(PD3)) {
		FFT_ComplexBuffer[bitswap[ADC_bufferIndex]].re = ADCH>>3;
		FFT_ComplexBuffer[bitswap[ADC_bufferIndex++]].im = 0;
	}else{
		FFT_ComplexBuffer[ADC_bufferIndex].re = ADCH>>3;
		FFT_ComplexBuffer[ADC_bufferIndex++].im = 0;
	}

	ADC_bufferIndex &= 0b01111111;
}

void ADC_init() {
     ADMUX  = 0b01100000;
     ADMUX |= 0; //ADCx
     ADCSRA = _BV(ADLAR) | 7;// 4 or 7
     ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADATE); //ADC enable, autotrigger
}

void ADC_takeSapmples() {
	ADCSRA |= _BV(ADSC); // Start
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
/*
	for(uint8_t i=0; i<FFT_LENGHT; ++i) {
		//float temp = 32.0f+12.0f*sin(2.0f*M_PI*40.0f*((1.0f/Fs)*i))+15.0f*sin(2.0f*M_PI*450.0f*((1.0f/Fs)*i));
		FFT_ComplexBuffer[bitswap[i]].re = data[i];
		FFT_ComplexBuffer[bitswap[i]].im = 0;
	}
	sei();
	FFT_calculate();
	uart_puts("Start\r\n");
	for(uint8_t i=0; i<128; ++i) {
		sprintf(text, "%4d\t%4d\r\n", FFT_ComplexBuffer[i].re, FFT_ComplexBuffer[i].im);
		uart_puts(text);
	}
	while(1);
*/
	LCD_init();
	ADC_init();
	DDRD &= ~(_BV(PD3));

	sei();

	while(1) {
		ADC_takeSapmples();

		// Print all
		LCD_clear();
		if(PIND & _BV(PD3)) {
			uart_puts("FFT START!!!!!\r\n");
			FFT_calculate();
			uart_puts("FFT END!!!!!\r\n");
			stem();
				
		}else{
			uart_puts("OSC\r\n");
			plot();
		}
		LCD_showBuffer();
/*
	for(uint8_t i=0; i<128; ++i) {
		sprintf(text, "%4d\t%4d\r\n", FFT_ComplexBuffer[i].re, FFT_ComplexBuffer[i].im);
		uart_puts(text);
	}
*/
		_delay_ms(10);
	}

	while(1);

	//   AND THE ULTIMATE ANSWER IIIISSS !!!

	//  	#   #  ####
	//	#   #     #
	//	#####  ####
	//	    #  #
	//	    #  ####

			// Rainbowdash is the best pony !

}
