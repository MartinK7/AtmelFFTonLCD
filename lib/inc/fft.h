#define FFT_LENGHT 128
#include <avr/pgmspace.h>

extern const uint8_t bitswap[FFT_LENGHT];

typedef struct{
    int16_t re;
    int16_t im;
}TCplx;

TCplx FFT_ComplexBuffer[FFT_LENGHT];
TCplx FFT_ComplexBuffer_2[FFT_LENGHT];

void FFT_calculate();

