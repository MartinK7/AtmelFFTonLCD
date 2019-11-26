
#include  "timer.h"
#include <avr/interrupt.h>


void  TIM_config_prescaler(uint8_t  timer_name, uint8_t  presc_val) {
    switch(timer_name) {
        case TIM0:
            switch(presc_val) {
                case TIM_PRESC_1:
                    TCCR0B &= 0b11111000;
//                  TCCR0B |= 0b000;
                    break;
                case TIM_PRESC_8:
                    TCCR0B &= 0b11111000;
                    TCCR0B |= 0b010;
                    break;
/*              case TIM_PRESC_32:
                    TCCR0B &= 0b11111000;
                    TCCR0B |= 0;
                    break;
*/
                case TIM_PRESC_64:
                    TCCR0B &= 0b11111000;
                    TCCR0B |= 0b011;
                    break;
/*              case TIM_PRESC_128:
                    TCCR0B &= 0b11111000;
                    TCCR0B |= 0;
                    break;
*/
                case TIM_PRESC_256:
                    TCCR0B &= 0b11111000;
                    TCCR0B |= 0b100;
                    break;
                case TIM_PRESC_1024:
                    TCCR0B &= 0b11111000;
                    TCCR0B |= 0b101;
                    break;
            }
            break;
        case TIM1:
            switch(presc_val) {
                case TIM_PRESC_1:
                    TCCR1B &= 0b11111000;
//                  TCCR1B |= 0b000;
                    break;
                case TIM_PRESC_8:
                    TCCR1B &= 0b11111000;
                    TCCR1B |= 0b010;
                    break;
/*              case TIM_PRESC_32:
                    TCCR1B &= 0b11111000;
                    TCCR1B |= 0;
                    break;
*/
                case TIM_PRESC_64:
                    TCCR1B &= 0b11111000;
                    TCCR1B |= 0b011;
                    break;
/*              case TIM_PRESC_128:
                    TCCR1B &= 0b11111000;
                    TCCR1B |= 0;
                    break;
*/
                case TIM_PRESC_256:
                    TCCR1B &= 0b11111000;
                    TCCR1B |= 0b100;
                    break;
                case TIM_PRESC_1024:
                    TCCR1B &= 0b11111000;
                    TCCR1B |= 0b101;
                    break;
            }
            break;
    }
}

void  TIM_config_interrupt(uint8_t  timer_name, uint8_t  interr_mode) {
    switch(timer_name) {
        case TIM0:
            switch(interr_mode)  {
                case TIM_OVERFLOW_DISABLE:
                    TIMSK0 &= ~_BV(TOIE0);
                    break;
                case TIM_OVERFLOW_ENABLE:
                    TIMSK0 |= _BV(TOIE0);
                    break;
            }
            break;
        case TIM1:
            switch(interr_mode)  {
                case TIM_OVERFLOW_DISABLE:
                    TIMSK1 &= ~_BV(TOIE1);
                    break;
                case TIM_OVERFLOW_ENABLE:
                    TIMSK1 |= _BV(TOIE1);
                    break;
            }
            break;
    }
}