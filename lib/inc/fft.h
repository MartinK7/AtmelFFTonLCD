
#define FFT_LENGHT 128
#define N 128

typedef struct{
    int16_t re;
    int16_t im;
}TCplx;

void FFT_pushDataSwap(uint8_t value, uint8_t index);
void FFT_pushData(uint8_t value, uint8_t index);
TCplx* FFT_getPtrData();
void FFT_calculate();

