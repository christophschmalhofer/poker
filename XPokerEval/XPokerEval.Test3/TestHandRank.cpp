// TestHandRank.cpp
// by Ray Wotton, Steve Brecher, and the 2+2 list

// Which hand evaluator to use:
#define LOOKUP      0   //Ray Wooten (RayW)'s lookup-table-based per mykey1961's idea as
                        // posted on twoplustwo.com "7 Card Hand Evaluator" thread:
                        // http://forumserver.twoplustwo.com/showflat.php?Cat=0&Board=software&Number=8513906&page=0&fpart=all
#define HANDEVAL    0   //Steve Brecher's HandEval -- http://www.stevebrecher.com
#define EVALN       1   //pokersource EVAL_N -- http://pokersource.sourceforge.net/

// Which test to run:
#define RANDOM1M    1   //one million random hands pre-stored in an array
#define ENUMERATE   0   //all combinations of 7 cards

#if LOOKUP+HANDEVAL+EVALN != 1
#error Not exactly one evaluator defined
#endif

#if RANDOM1M+ENUMERATE != 1
#error Not exactly one test defined
#endif

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream> // just for cin.get
#include "..\XPokerEval.RNG\mtrand.h" // mersenne twister RNG

const char HandRanks[][16] = {"BAD!!","High Card","Pair","Two Pair","Three of a Kind","Straight","Flush","Full House","Four of a Kind","Straight Flush"};

LARGE_INTEGER timings, endtimings;	// for good timing

#if RANDOM1M
//#include "RNG.h"    //a "good" RNG I use, but C library's rand could be used also (SB)
#endif

#if LOOKUP
int HR[32487834];  // hold the handranks
int hands[1000000][7];
#elif HANDEVAL
#include "..\XPokerEval.SteveBrecher\HandEval.h"
Hand_T hands[1000000];
#elif EVALN
#include "eval.h"
StdDeck_CardMask hands[1000000];
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0;
	int handTypeSum[10];
#if ENUMERATE
	int c0, c1, c2, c3, c4, c5, c6;
#if LOOKUP
	int u0, u1, u2, u3, u4, u5;
#endif
#endif

	memset(handTypeSum, 0, sizeof(handTypeSum));  // do init..

#if LOOKUP
	memset(HR, 0, sizeof(HR));
	FILE * fin = fopen("HandRanks.dat", "rb"); // get the hand array saved by HandRankSetup
	if (!fin) {
        fin = fopen("..\\HandRanks.dat", "rb");
        if (!fin) {									   // problem say so
		    printf("Problem Opening Input File!\n");
		    return 1;
        }
	}
	size_t bytesread = fread(HR, sizeof(HR), 1, fin);	// get the HandRank Array
	fclose(fin);										// close up


	// This loop is going to initialize the virginDeck array
	// to ranks within suits, meaning, the 1st three members
	// of the virginDeckStructure will be:
	//
	// { 1, 5,  9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49
	//   2, 6, 10, 14, 18, 22, 26, 32, 36, 40, 44, 48, 52, ... }
	//
	// which equates to:
	//
	// { 2c, 3c, 4c, 5c, 6c, 7c, 8c, 9c, 10c, Jc, Qc, Kc, Ac 
	//   2d, 3d, 4d, 5d, 6d, 7d, 8d, 9d, 10d, Jd, Qd, Kd, Ad, ... }
	

    int virginDeck[52];
    for (int c = 0; c < 52; c++) {
        //re-order deck to ranks within suits (per Eval_N and
        //Hand_7_Eval -- majority rule :) so all evaluators get
        //same hands (suits may be transposed, with no effect)
        //for the RANDOM1M test.
        int rank = c%13;  //0-based, not deuce-based
        int suit = c/13;
        virginDeck[c] = rank*4 + suit + 1; //+1 because 1-based
    }
#elif HANDEVAL
    Hand_T deck[52], virginDeck[52];
    for (int c = 0; c < 52; c++)
        virginDeck[c].as64Bits = IndexToMask(c);
#elif EVALN
    StdDeck_CardMask virginDeck[52], deck[52];
    for (int c = 0; c < 52; c++)
        virginDeck[c] = StdDeck_MASK(c);
#endif

#if RANDOM1M
    printf("\rDealing hands...");

	// Set up the RNG with some random data.. to get truly random you'd want to
	// populate this with bytes pulled from the crypto API.
	unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
	MTRand53 irand(init, length); 	// Generates 53-bit precision doubles between [0,1)

#if LOOKUP
	int deck[52];
#endif

	// This loop is going to create an in-memory array of 1,000,000 hands.
	// This is the hands[7][1000000] above
    for (int i = 0; i < 1000000; i++)
	{
        memmove(deck, virginDeck, sizeof(deck));

		// Pick a random number between 0 and 51 inclusive
		int r = irand.under(52);

		// Pick the first card of the hand
#if LOOKUP
        hands[i][0] = deck[r];
#elif EVALN
        StdDeck_CardMask h = deck[r];
#elif HANDEVAL
        Hand_T h = deck[r];
#endif

		// Pick the remaining 6 cards
        for (int c = 1; c < 7; c++)
		{
            memmove(&deck[r], &deck[r+1], (52-c-r)*sizeof(deck[0])); // remove dealt card
			r = irand.under(52-c);
#if LOOKUP
            hands[i][c] = deck[r];
        }
#else
#if EVALN
            StdDeck_CardMask_OR(h, h, deck[r]);
#elif HANDEVAL
            h.as64Bits |= deck[r].as64Bits;
#endif
        }
        hands[i] = h;
#endif
    }
    printf("\n");
#endif

#if HANDEVAL
    Init_Hand_Eval();
#endif

	clock_t timer = clock();						    // start regular clock

    QueryPerformanceCounter(&timings);				    // start High Precision clock

	int diffcount = 0;  //TODO

#if RANDOM1M
#if LOOKUP
    int* h = hands[0];
#endif
    for (int i = 0; i < 1000000; i++)
	{
#if LOOKUP
        int p = HR[53 + *h++];
        p = HR[p + *h++];
        p = HR[p + *h++];
        p = HR[p + *h++];
        p = HR[p + *h++];
        p = HR[p + *h++];

        handTypeSum[HR[p + *h++] >> 12]++;
#elif HANDEVAL
        handTypeSum[(Hand_7_Eval(hands[i]) >> 24)+1]++;
#elif EVALN
        handTypeSum[HandVal_HANDTYPE(StdDeck_StdRules_EVAL_N(hands[i], 7))+1]++;
#endif
        count++;
    }
#else
#if LOOKUP
    // with reduced loop limits [SB]
	for (c0 = 1; c0 < 47; c0++) {
		u0 = HR[53+c0];
		for (c1 = c0+1; c1 < 48; c1++) {
			u1 = HR[u0+c1];
 			for (c2 = c1+1; c2 < 49; c2++) {
				u2 = HR[u1+c2];
				for (c3 = c2+1; c3 < 50; c3++) {
					u3 = HR[u2+c3];
 					for (c4 = c3+1; c4 < 51; c4++) {
						u4 = HR[u3+c4];
						for (c5 = c4+1; c5 < 52; c5++) {
							u5 = HR[u4+c5];
 							for (c6 = c5+1; c6 < 53; c6++) {
								handTypeSum[HR[u5+c6] >> 12]++;
								count++;
							}
						}
					}
				}
			}
		}
	}
#elif HANDEVAL
    Hand_T h0, h1, h2, h3, h4, h5, h6;
	for (c0 = 0; c0 < 46; c0++) {
		h0 = virginDeck[c0];
		for (c1 = c0+1; c1 < 47; c1++) {
			h1.as64Bits = h0.as64Bits | virginDeck[c1].as64Bits;
 			for (c2 = c1+1; c2 < 48; c2++) {
				h2.as64Bits = h1.as64Bits | virginDeck[c2].as64Bits;
				for (c3 = c2+1; c3 < 49; c3++) {
					h3.as64Bits = h2.as64Bits | virginDeck[c3].as64Bits;
 					for (c4 = c3+1; c4 < 50; c4++) {
						h4.as64Bits = h3.as64Bits | virginDeck[c4].as64Bits;
						for (c5 = c4+1; c5 < 51; c5++) {
							h5.as64Bits = h4.as64Bits | virginDeck[c5].as64Bits;
 							for (c6 = c5+1; c6 < 52; c6++) {
                                h6.as64Bits = h5.as64Bits | virginDeck[c6].as64Bits;
								handTypeSum[(Hand_7_Eval(h6) >> 24)+1]++;
								count++;
							}
						}
					}
				}
			}
		}
	}
#elif EVALN
    StdDeck_CardMask h0, h1, h2, h3, h4, h5, h6;
	for (c0 = 0; c0 < 46; c0++) {
		h0 = virginDeck[c0];
		for (c1 = c0+1; c1 < 47; c1++) {
			h1.cards_n = h0.cards_n | virginDeck[c1].cards_n;
 			for (c2 = c1+1; c2 < 48; c2++) {
				h2.cards_n = h1.cards_n | virginDeck[c2].cards_n;
				for (c3 = c2+1; c3 < 49; c3++) {
					h3.cards_n = h2.cards_n | virginDeck[c3].cards_n;
 					for (c4 = c3+1; c4 < 50; c4++) {
						h4.cards_n = h3.cards_n | virginDeck[c4].cards_n;
						for (c5 = c4+1; c5 < 51; c5++) {
							h5.cards_n = h4.cards_n | virginDeck[c5].cards_n;
 							for (c6 = c5+1; c6 < 52; c6++) {
                                h6.cards_n = h5.cards_n | virginDeck[c6].cards_n;
								handTypeSum[HandVal_HANDTYPE(StdDeck_StdRules_EVAL_N(h6, 7))+1]++;
								count++;
							}
						}
					}
				}
			}
		}
	}
#endif
#endif

    QueryPerformanceCounter(&endtimings);	  // end the high precision clock

	timer = clock() - timer;				  // end the regular clock

    printf(
#if LOOKUP
        "LOOKUP/"
#elif HANDEVAL
        "HANDEVAL/"
#elif EVALN
        "EVALN/"
#endif
        );
    printf(
#if RANDOM1M
        "RANDOM1M:"
#elif ENUMERATE
        "ENUMERATE:"
#endif
        );
	for (int i = 0; i <= 9; i++)			  // display results
		printf("\n%16s = %d", HandRanks[i], handTypeSum[i]);
	
	printf("\nTotal Hands = %d\n", count);

	__int64 clocksused = (__int64)endtimings.QuadPart - (__int64) timings.QuadPart;  // calc clocks used from the High Precision clock

	// and display the clock results
	printf("\nValidation seconds = %.4lf\nTotal HighPrecision Clocks = %I64d\nHighPrecision clocks per lookup = %lf\n", (double)timer/CLOCKS_PER_SEC, clocksused, (double) clocksused /  count) ;
	std::cin.get();

	return 0;
}
/*
for each evaluator/test, the best of three consecutive runs:

LOOKUP/RANDOM1M:
           BAD!! = 0
       High Card = 174303
            Pair = 438408
        Two Pair = 235438
 Three of a Kind = 47901
        Straight = 46004
           Flush = 30026
      Full House = 26001
  Four of a Kind = 1632
  Straight Flush = 287
Total Hands = 1000000

Validation seconds = 0.5780
Total HighPrecision Clocks = 1977461472
HighPrecision clocks per lookup = 1977.461472

HANDEVAL/RANDOM1M:
           BAD!! = 0
       High Card = 174303
            Pair = 438408
        Two Pair = 235438
 Three of a Kind = 47901
        Straight = 46004
           Flush = 30026
      Full House = 26001
  Four of a Kind = 1632
  Straight Flush = 287
Total Hands = 1000000

Validation seconds = 0.0310
Total HighPrecision Clocks = 112314720
HighPrecision clocks per lookup = 112.314720

EVALN/RANDOM1M:
           BAD!! = 0
       High Card = 174303
            Pair = 438408
        Two Pair = 235438
 Three of a Kind = 47901
        Straight = 46004
           Flush = 30026
      Full House = 26001
  Four of a Kind = 1632
  Straight Flush = 287
Total Hands = 1000000

Validation seconds = 0.0310
Total HighPrecision Clocks = 91655640
HighPrecision clocks per lookup = 91.655640

LOOKUP/ENUMERATE:
           BAD!! = 0
       High Card = 23294460
            Pair = 58627800
        Two Pair = 31433400
 Three of a Kind = 6461620
        Straight = 6180020
           Flush = 4047644
      Full House = 3473184
  Four of a Kind = 224848
  Straight Flush = 41584
Total Hands = 133784560

Validation seconds = 0.6400
Total HighPrecision Clocks = 2157517656
HighPrecision clocks per lookup = 16.126806

HANDEVAL/ENUMERATE:
           BAD!! = 0
       High Card = 23294460
            Pair = 58627800
        Two Pair = 31433400
 Three of a Kind = 6461620
        Straight = 6180020
           Flush = 4047644
      Full House = 3473184
  Four of a Kind = 224848
  Straight Flush = 41584
Total Hands = 133784560

Validation seconds = 3.1400
Total HighPrecision Clocks = 10640291544
HighPrecision clocks per lookup = 79.533031

EVALN/ENUMERATE:
           BAD!! = 0
       High Card = 23294460
            Pair = 58627800
        Two Pair = 31433400
 Three of a Kind = 6461620
        Straight = 6180020
           Flush = 4047644
      Full House = 3473184
  Four of a Kind = 224848
  Straight Flush = 41584
Total Hands = 133784560

Validation seconds = 3.2180
Total HighPrecision Clocks = 10912503860
HighPrecision clocks per lookup = 81.567737
*/
