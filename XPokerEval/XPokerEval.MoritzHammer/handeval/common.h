#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>

/** Helper macros */
#define min(A,B) (((A)<(B))?(A):(B))
#ifndef NOASSERT
	#define assert(A) if (!(A)) { printf("Assertion failure at %s:%u: %s\n",  __FILE__, __LINE__, #A); exit(-1);}
#else
	#define assert(A)
#endif 

/** Card specific constants */
#define HANDTYPECLASSCOUNT 9
static char CARD[13] = {'2', '3', '4' ,'5', '6', '7', '8', '9', 'T' , 'J', 'Q', 'K', 'A'};
static char COLOR[4] = {'H', 'S', 'C', 'D'};
static char *HANDTYPE[HANDTYPECLASSCOUNT] = {"HC", "1P", "2P", "3K", "S", "F", "FH", "4K", "SF"};

#endif
