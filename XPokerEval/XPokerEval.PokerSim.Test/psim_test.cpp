#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <windows.h>
#include "..\XPokerEval.PokerSim\psim.hpp"
#include "..\XPokerEval.Pokersource\include\inlines\eval.h"
//#include "..\XPokerEval.PokerSim\simulate.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	int hand[7];
	int handCategoryFreq[9];
	memset(handCategoryFreq, 0, sizeof(handCategoryFreq));
	unsigned int handRank = 0;
	int count = 0;

	printf("Testing: Timmy's PokerSim Evaluator\n");
	printf("-----------------------------------\n\n");
	printf("Enumerating and evaluating all 133,784,560 possible 7-card hands...\n");

	DWORD dwTime = GetTickCount();

	// Let's loop over all 7-card combinations...
	for (hand[0] = 0; hand[0] < 46; hand[0]++) {
		for (hand[1] = hand[0]+1; hand[1] < 47; hand[1]++) {
 			for (hand[2] = hand[1]+1; hand[2] < 48; hand[2]++) {
				for (hand[3] = hand[2]+1; hand[3] < 49; hand[3]++) {
 					for (hand[4] = hand[3]+1; hand[4] < 50; hand[4]++) {
						for (hand[5] = hand[4]+1; hand[5] < 51; hand[5]++) {
 							for (hand[6] = hand[5]+1; hand[6] < 52; hand[6]++)
							{
								// RankHand is a PokerSim function..
								// HandVal_HANDTYPE is a Pokersource poker-eval macro...
								handCategoryFreq[HandVal_HANDTYPE(RankHand(hand))]++;
								
								// Above line is equivalent to...
								//int handRank = RankHand(hand);
								//int handCategory = HandVal_HANDTYPE(handRank);
								//handCategoryFreq[handCategory]++;

								count++;
							}
						}
					}
				}
			}
		}
	}

	dwTime = GetTickCount() - dwTime;

	printf("High Card:        %d\n", handCategoryFreq[0]);
	printf("One Pair:         %d\n", handCategoryFreq[1]);
	printf("Two Pair:         %d\n", handCategoryFreq[2]);
	printf("Trips:            %d\n", handCategoryFreq[3]);
	printf("Straight:         %d\n", handCategoryFreq[4]);
	printf("Flush:            %d\n", handCategoryFreq[5]);
	printf("Full House:       %d\n", handCategoryFreq[6]);	
	printf("Quads:            %d\n", handCategoryFreq[7]);
	printf("Straight Flush:   %d\n", handCategoryFreq[8]);
	printf("Enumerated and evaluated %d hands in %d milliseconds!", count, dwTime);

	printf("\n\nNow let's run some hand vs. hand matchups...\n\n");

	printf("MATCHUP #1: AsKs vs. Random Hand, Flop = Kd Qd Qc\n");
	SimResults r;
	SimulateHand("As Ks    Kd Qd Qc", &r);
	printf("AsKs wins %.1f%% of the time.",r.winSd * 100);

	printf("\n\nMATCHUP #2: 9c9h vs. Random Hand, Preflop\n");
	SimulateHand("9c 9h", &r);
	printf("9c9h wins %.1f%% of the time.",r.winSd * 100);



	std::cin.get();

	return 0;
}

