#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <fft.h>

// Swap
const uint8_t bitswap[FFT_LENGHT] = {
      0,  64,  32,  96,  16,  80,  48, 112,   8,  72, 
     40, 104,  24,  88,  56, 120,   4,  68,  36, 100, 
     20,  84,  52, 116,  12,  76,  44, 108,  28,  92, 
     60, 124,   2,  66,  34,  98,  18,  82,  50, 114, 
     10,  74,  42, 106,  26,  90,  58, 122,   6,  70, 
     38, 102,  22,  86,  54, 118,  14,  78,  46, 110, 
     30,  94,  62, 126,   1,  65,  33,  97,  17,  81, 
     49, 113,   9,  73,  41, 105,  25,  89,  57, 121, 
      5,  69,  37, 101,  21,  85,  53, 117,  13,  77, 
     45, 109,  29,  93,  61, 125,   3,  67,  35,  99, 
     19,  83,  51, 115,  11,  75,  43, 107,  27,  91, 
     59, 123,   7,  71,  39, 103,  23,  87,  55, 119, 
     15,  79,  47, 111,  31,  95,  63, 127
};

//64 bigger
const float TSinCos[FFT_LENGHT] = {
    0.0000f, 1.0000f, 0.0491f, 0.9988f, 0.0980f, 0.9952f, 
    0.1467f, 0.9892f, 0.1951f, 0.9808f, 0.2430f, 0.9700f, 
    0.2903f, 0.9569f, 0.3369f, 0.9415f, 0.3827f, 0.9239f, 
    0.4276f, 0.9040f, 0.4714f, 0.8819f, 0.5141f, 0.8577f, 
    0.5556f, 0.8315f, 0.5957f, 0.8032f, 0.6344f, 0.7730f, 
    0.6716f, 0.7410f, 0.7071f, 0.7071f, 0.7410f, 0.6716f, 
    0.7730f, 0.6344f, 0.8032f, 0.5957f, 0.8315f, 0.5556f, 
    0.8577f, 0.5141f, 0.8819f, 0.4714f, 0.9040f, 0.4276f, 
    0.9239f, 0.3827f, 0.9415f, 0.3369f, 0.9569f, 0.2903f, 
    0.9700f, 0.2430f, 0.9808f, 0.1951f, 0.9892f, 0.1467f, 
    0.9952f, 0.0980f, 0.9988f, 0.0491f, 1.0000f, 0.0000f, 
    0.9988f, -0.0491f, 0.9952f, -0.0980f, 0.9892f, -0.1467f, 
    0.9808f, -0.1951f, 0.9700f, -0.2430f, 0.9569f, -0.2903f, 
    0.9415f, -0.3369f, 0.9239f, -0.3827f, 0.9040f, -0.4276f, 
    0.8819f, -0.4714f, 0.8577f, -0.5141f, 0.8315f, -0.5556f, 
    0.8032f, -0.5957f, 0.7730f, -0.6344f, 0.7410f, -0.6716f, 
    0.7071f, -0.7071f, 0.6716f, -0.7410f, 0.6344f, -0.7730f, 
    0.5957f, -0.8032f, 0.5556f, -0.8315f, 0.5141f, -0.8577f, 
    0.4714f, -0.8819f, 0.4276f, -0.9040f, 0.3827f, -0.9239f, 
    0.3369f, -0.9415f, 0.2903f, -0.9569f, 0.2430f, -0.9700f, 
    0.1951f, -0.9808f, 0.1467f, -0.9892f, 0.0980f, -0.9952f, 
    0.0491f, -0.9988f
};

TCplx signal[128];

void calbut(TCplx *A, TCplx *B, float sinus, float cosinus) {
    TCplx C;

    C.re=B->im*sinus + B->re*cosinus;
    C.im=B->im*cosinus - B->re*sinus;
    B->re=A->re - C.re;
    B->im=A->im - C.im;
    A->re=A->re + C.re;
    A->im=A->im + C.im;
}

TCplx* FFT_getPtrData() {
    return signal;
}

void FFT_pushDataSwap(uint8_t value, uint8_t index) {
    if(index > 127)return;
    signal[bitswap[index]].re = (int16_t)value;
    signal[bitswap[index]].im = 0.0f;
}

void FFT_pushData(uint8_t value, uint8_t index) {
    if(index > 127)return;
    signal[index].re = (int16_t)value;
    signal[index].im = 0.0f;
}

void FFT_calculate() {
    float Fs = 1000.0f;

    // Generate signal + bitswap
    for(uint8_t i=0; i<FFT_LENGHT; ++i) {
        float temp = 32.0f+12.0f*sin(2.0f*M_PI*40.0f*((1.0f/Fs)*i))+20.0f*sin(2.0f*M_PI*450.0f*((1.0f/Fs)*i));
        signal[bitswap[i]].re = (int16_t)temp;
        signal[i].im = 0.0f;
    }

    // Round
    float dsin,dcos;
    const float *Fnk;
    TCplx C;
    TCplx *A,*B;
    int32_t s,k;
    int32_t n=1;
    int32_t angf=N/2;
    while(n<N)
    {
        A=&signal[0];
        B=&signal[n];     
        Fnk=TSinCos;
        
        for(k=0;k<n;k++)
        {
            dsin=*Fnk++;
            dcos=*Fnk++;
            
            for(s=0;s<angf;s++)
            {
                C.re=B->im*dsin + B->re*dcos;
                C.im=B->im*dcos - B->re*dsin;
                B->re=A->re - C.re;
                B->im=A->im - C.im;
                A->re=A->re + C.re;
                A->im=A->im + C.im;
                A+=2*n;
                B+=2*n;              
            }
            A-=(N-1);
            B-=(N-1);             
            Fnk+=(angf-1)*2;
        }        
        n<<=1;
        angf>>=1;
    }

    // Abs
    for(uint8_t i=0; i<FFT_LENGHT; ++i) {
        float a = signal[i].re;
        float b = signal[i].im;
        signal[i].re = sqrt(a*a+b*b);
    }
}