#ifndef __RANDOMGENERATOR_H__
#define __RANDOMGENERATOR_H__

//#define USE_MERSENNE_TWISTER
#define BUFFER_SIZE 65536
//#define USE_BUFFER

void init_random(unsigned int seed);
char get_random_card(char max);

#endif

