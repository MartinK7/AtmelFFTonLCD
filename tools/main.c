#include <stdio.h>
#include <stdint.h>

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
	printf("\n");
}
