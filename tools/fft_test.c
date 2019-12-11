#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define FFT_LENGHT 128

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
const int8_t TSinCos[FFT_LENGHT] = {   0,    64, 
    3,    63,     6,    63,     9,    63,    12,    62, 
   15,    62,    18,    61,    21,    60,    24,    59, 
   27,    57,    30,    56,    32,    54,    35,    53, 
   38,    51,    40,    49,    42,    47,    45,    45, 
   47,    42,    49,    40,    51,    38,    53,    35, 
   54,    32,    56,    30,    57,    27,    59,    24, 
   60,    21,    61,    18,    62,    15,    62,    12, 
   63,     9,    63,     6,    63,     3,    64,     0, 
   63,    -3,    63,    -6,    63,    -9,    62,   -12, 
   62,   -15,    61,   -18,    60,   -21,    59,   -24, 
   57,   -27,    56,   -30,    54,   -32,    53,   -35, 
   51,   -38,    49,   -40,    47,   -42,    45,   -45, 
   42,   -47,    40,   -49,    38,   -51,    35,   -53, 
   32,   -54,    30,   -56,    27,   -57,    24,   -59, 
   21,   -60,    18,   -61,    15,   -62,    12,   -62, 
    9,   -63,     6,   -63,     3,   -63};

typedef struct{
    int16_t re;
    int16_t im;
}TCplx;

TCplx FFT_ComplexBuffer[FFT_LENGHT];

uint8_t data[FFT_LENGHT] = {0,0,1,13,30,31,31,24,6,0,0,0,0,0,0,2,18,31,31,30,14,2,0,0,0,8,27,31,31,30,15,2,0,0,0,0,0,2,15,30,31,31,23,6,0,0,0,0,0,0,5,23,30,31,30,14,2,0,0,0,0,0,1,31,30,13,2,0,0,0,0,0,2,16,31,31,31,23,6,0,0,0,0,0,0,6,23,31,31,30,14,2,0,0,0,0,0,2,14,30,31,1,0,0,0,0,0,2,15,30,31,30,21,6,0,0,0,0,0,0,7,26,31,31,30,13,2,0};

void main() {
    float Fs = 1000.0f;

    // Generate FFT_ComplexBuffer + bitswap
    for(uint8_t i=0; i<FFT_LENGHT; ++i) {
        //float temp = 32.0f+12.0f*sin(2.0f*M_PI*40.0f*((1.0f/Fs)*i))+15.0f*sin(2.0f*M_PI*450.0f*((1.0f/Fs)*i));
        FFT_ComplexBuffer[bitswap[i]].re = data[i];
        FFT_ComplexBuffer[bitswap[i]].im = 0;
    }

    // Round
    int32_t dsin, dcos;
    const int8_t *Fnk;
    TCplx C;
    TCplx *A,*B;
    uint8_t s,k;
    uint8_t n=1;
    uint8_t angf=FFT_LENGHT/2;
    while(n<FFT_LENGHT)
    {
        A=&FFT_ComplexBuffer[0];
        B=&FFT_ComplexBuffer[n];     
        Fnk=TSinCos;
        
        for(k=0;k<n;k++)
        {
            dsin=*Fnk++;
            dcos=*Fnk++;
            
            for(s=0;s<angf;s++)
            {
                C.re=B->im*dsin/64 + B->re*dcos/64;
                C.im=B->im*dcos/64 - B->re*dsin/64;
                B->re=A->re - C.re;
                B->im=A->im - C.im;
                A->re=A->re + C.re;
                A->im=A->im + C.im;
                A+=2*n;
                B+=2*n;              
            }
            A-=(FFT_LENGHT-1);
            B-=(FFT_LENGHT-1);             
            Fnk+=(angf-1)*2;
        }        
        n<<=1;
        angf>>=1;
    }

    // Abs
    for(uint8_t i=0; i<FFT_LENGHT; ++i) {
        int16_t a = FFT_ComplexBuffer[i].re;
        int16_t b = FFT_ComplexBuffer[i].im;
        FFT_ComplexBuffer[i].re = sqrt(a*a+b*b);
        FFT_ComplexBuffer[i].im = 0;
    }

    // Print all
    for(uint8_t i=0; i<FFT_LENGHT; ++i) {
        printf("%4d ", FFT_ComplexBuffer[i].re);
    }
    
}
