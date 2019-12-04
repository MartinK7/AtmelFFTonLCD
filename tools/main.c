#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main(void) {

	uint32_t bits = 10;
	uint32_t power = 1024; //2^bits

	scanf("%d", &bits);
	power = (1<<bits);

	for(uint32_t i=0; i<power; ++i) {
		uint32_t temp = 0;
		for(uint32_t bit=0; bit<bits; ++bit) {
			if(i&(1<<bit))
				temp |= (1<<(bits-1-bit));
		}
		printf("%3d", temp);
		if((i+1) != power)
			printf(", ");
		if(!((i+1)%10) && i!=0)
			printf("\n");
	}
	printf("\n\n");
	uint32_t cnt = 0;
	for(uint32_t m=0; m<bits; ++m) {
		for(uint32_t k=0; k<(1<<m); ++k) {
			uint32_t T = (2<<m);
			float Fsin = sin((2*M_PI*(float)k)/(float)T);
			float Fcos = cos((2*M_PI*(float)k)/(float)T);
			printf("%5d, %5d, ", (int16_t)(Fsin*64.0f), (int16_t)(Fcos*64.0f));
			++cnt;	
			if(cnt%4==0 && cnt!=0) {
				printf("\n");cnt=0;}
		}
	}
	printf("\n\n");
	for(uint32_t m=0; m<bits; ++m) {
		for(uint32_t k=0; k<(1<<m); ++k) {
			uint32_t T = (2<<m);
			float Fsin = sin((2*M_PI*(float)k)/(float)T);
			float Fcos = cos((2*M_PI*(float)k)/(float)T);
			printf("%.4ff, %.4ff, ", (Fsin), (Fcos));
			++cnt;	
			if(cnt%4==0 && cnt!=0) {
				printf("\n");cnt=0;}
		}
	}

	printf("\n");
}
