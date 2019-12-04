
#define FFT_LENGHT 128
#define N 128

typedef struct{
    int16_t re;
    int16_t im;
}TCplx;

TCplx* FFT_getPtrData();
void FFT_calculate();

