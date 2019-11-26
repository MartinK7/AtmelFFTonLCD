/***********************************************************************
 * Title:    Seven-segment display library
 * Author:   Tomas Fryza, Brno University of Technology, Czechia
 * Software: avr-gcc, tested with avr-gcc 4.9.2
 * Hardware: Any AVR
 *
 * MIT License
 *
 * Copyright (c) 2019 Tomas Fryza
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Define ------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
/* Active low digit 0 to 9 + A to F */
uint8_t segment_digit[] = {
   //DPgfedcba
    0b11000000,      // Digit 0
    0b11111001,      // Digit 1
    0b10100100,      // Digit 2
    0b10110000,
    0b10011001,
    0b10010010,
    0b10000010,
    0b11111000,
    0b10000000,
    0b10010000,
    0b10001000,
    0b10000011,
    0b11000110,
    0b10100001,
    0b10000110,
    0b10001110
};


/* Active high position 0 to 3 */
uint8_t segment_position[] = {
    0b00001000,   // Position 0
    0b00000100,   // Position 1
    0b00000010,   // Position 2
    0b00000001};  // Position 3

uint8_t segment_data[4] = {13, 14, 15, 0};
uint8_t index = 0;

uint8_t* SEG_getPtrData()
{
    return segment_data;
}

void SEG_next()
{
    index = (index + 1) & 0b00000011;
    SEG_putc (segment_data[index], 3-index);
}

/* Functions ---------------------------------------------------------*/
void SEG_putc(uint8_t digit,
              uint8_t position)
{
    uint8_t i;

    /* Read values from look-up tables */
    digit    = segment_digit[digit];
    position = segment_position[position];

    /* Put 1st byte to serial data */
    for (i = 0; i < 8; i++) {
        // TODO: Test and send 8 individual "digit" bits
        GPIO_write(&PORTB, SEGMENT_DATA, digit & _BV(7-i));
        SEG_toggle_clk();
    }

    /* Put 2nd byte to serial data */
    for (i = 0; i < 8; i++) {
        // TODO: Test and send 8 individual "position" bits
        GPIO_write(&PORTB, SEGMENT_DATA, position & _BV(7-i));
        //GPIO_write(&PORTB, SEGMENT_DATA, 1);
        SEG_toggle_clk();
    }

    /* TODO: Generate 1 us latch pulse */
    GPIO_write (&PORTD, SEGMENT_LATCH, 1);
    _delay_us(1);
    GPIO_write (&PORTD, SEGMENT_LATCH, 0);
    _delay_us(1);

}

/*--------------------------------------------------------------------*/
void SEG_toggle_clk(void)
{
    /* TODO: Generate 2 us clock period */
    GPIO_write (&PORTD, SEGMENT_CLK, 1);
    _delay_us(1);
    GPIO_write (&PORTD, SEGMENT_CLK, 0);
    _delay_us(1);
}
