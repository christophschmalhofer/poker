// XPokerEval.Senzee7.Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\XPokerEval.Senzee7\buildtable.h"
#include "..\XPokerEval.Senzee7\index52c7.h"
#include "..\XPokerEval.CactusKev.PerfectHash\poker.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	printf("Testing the Senzee7 Hand Evaluator\n");
	printf("----------------------------------\n");

	short int* theTable = create_table("SENZEE7.DAT");
	if (!theTable)
		return -1;
	
	__int64 virginDeck[52];
	for (int c = 0; c < 52; c++)
		virginDeck[c] = (0x1LL << c);

	int freq[9];
	memset(freq, 0, sizeof(freq));

	int c0, c1, c2, c3, c4, c5, c6;
	__int64 h0, h1, h2, h3, h4, h5, h6;
	int count = 0;

	printf("\nEnumerating and evaluating all 133,784,560 million possible 7-card hands...\n\n");

	DWORD dwTime = GetTickCount();

	for (c0 = 0; c0 < 46; c0++) {
		h0 = virginDeck[c0]; // 1st card
		for (c1 = c0+1; c1 < 47; c1++) {
			h1 = h0 | virginDeck[c1]; // 2nd card
			for (c2 = c1+1; c2 < 48; c2++) {
				h2 = h1 | virginDeck[c2]; // 3rd card
				for (c3 = c2+1; c3 < 49; c3++) {
					h3 = h2 | virginDeck[c3]; // 4th card
					for (c4 = c3+1; c4 < 50; c4++) {
						h4 = h3 | virginDeck[c4]; // 5th card
						for (c5 = c4+1; c5 < 51; c5++) {
							h5 = h4 | virginDeck[c5]; // 6th card
							for (c6 = c5+1; c6 < 52; c6++) {
								h6 = h5 | virginDeck[c6]; // 7th card

								freq[9 - hand_rank(theTable[index52c7(h6)])]++;
								// Above line could be re-written as:
								// int hashedIndex = index52c7(h6);
								// int handValue = theTable[hashedIndex];
								// int handCategory = hand_rank(handValue);
								// freq[9 - handCategory]++;

								count++;
							}
						}
					}
				}
			}
		}
	}

	dwTime = GetTickCount() - dwTime;

	printf("High Card:        %d\n", freq[0]);
	printf("One Pair:         %d\n", freq[1]);
	printf("Two Pair:         %d\n", freq[2]);
	printf("Trips:            %d\n", freq[3]);
	printf("Straight:         %d\n", freq[4]);
	printf("Flush:            %d\n", freq[5]);
	printf("Full House:       %d\n", freq[6]);	
	printf("Quads:            %d\n", freq[7]);
	printf("Straight Flush:   %d\n", freq[8]);

	printf("\nEnumerated and evaluated all %d poker hands in %d milliseconds!", count, dwTime);

	std::cin.get();


	return 0;
}

