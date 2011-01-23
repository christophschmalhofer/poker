#include "randomgenerator.h"
#include "stdlib.h"
//#include "SFMT.h"

#ifdef USE_BUFFER
char* buffer;
unsigned int bp;
#endif

void init_random(unsigned int seed) {
	#ifdef USE_MERSENNE_TWISTER
		init_gen_rand(seed);
		#ifdef USE_BUFFER
			buffer = (char*) malloc(BUFFER_SIZE);
			bp = BUFFER_SIZE;
		#endif
	#else
		srand(seed);
	#endif
}

char get_random_card(char maxv) {
	#ifdef USE_MERSENNE_TWISTER
		#ifdef USE_BUFFER
			if (bp >= BUFFER_SIZE) {
				fill_array32((uint32_t*) buffer, BUFFER_SIZE/4);
				bp = 0;
			}
			return ((buffer[bp++] & 127) >> 1) % maxv;
		#else
			return (char)(gen_rand32() % maxv);
		#endif
	#else
		return (char) (rand() % maxv);
	#endif
}
