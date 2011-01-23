#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "..\XPokerEval.MoritzHammer\handevaluator.h"


int _tmain(int argc, _TCHAR* argv[])
{
	printf("Moritz Hammer's DAG Evaluator\n");
	printf("-----------------------------\n\n");

	printf("Loading equivalence class and DAG data files...\n\n");
	// Initialize the hand eval library with the name of the equivalence class
	// data file and the directed acyclic graph (DAG) file.
	handeval_init("eqcllist", "carddag");

	int count = 0;
	int freq[9];
	memset(freq, 0, sizeof(freq));

	char hand[7];
	handeval_eq_class* eqClass = NULL;

	printf("Enumerating and evaluating all 133,784,560 possible 7-card poker hands...\n\n");

	DWORD dwTime = GetTickCount();

	for (hand[0] = 0; hand[0] < 46; hand[0]++) {
		for (hand[1] = hand[0]+1; hand[1] < 47; hand[1]++) {
 			for (hand[2] = hand[1]+1; hand[2] < 48; hand[2]++) {
				for (hand[3] = hand[2]+1; hand[3] < 49; hand[3]++) {
 					for (hand[4] = hand[3]+1; hand[4] < 50; hand[4]++) {
						for (hand[5] = hand[4]+1; hand[5] < 51; hand[5]++) {
 							for (hand[6] = hand[5]+1; hand[6] < 52; hand[6]++)
							{
								eqClass = calculate_equivalence_class(hand);
								freq[eqClass->type]++;
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

	printf("\nEnumerated and evaluated %d hands in %d milliseconds!", count, dwTime);

	std::cin.get();

	return 0;
}

