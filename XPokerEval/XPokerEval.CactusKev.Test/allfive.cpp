#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include "..\XPokerEval.CactusKev\poker.h"

/*************************************************/
/*                                               */
/* This code tests my evaluator, by looping over */
/* all 2,598,960 possible five card hands, cal-  */
/* culating each hand's distinct value, and dis- */
/* playing the frequency count of each hand type */
/*                                               */
/* Kevin L. Suffecool, 2001                      */
/* suffecool@bigfoot.com                         */
/*                                               */
/* James Devlin: added various comments, and a   */
/* 7-card test to this file.                     */
/*                                               */
/*************************************************/

int _tmain(int argc, _TCHAR* argv[])
{
    int deck[52], hand[7], freq[10];
    int c0,c1,c2,c3,c4,c5,c6;
	int equivClass, handCategory;

    // initialize the deck
    init_deck( deck );

    // zero out the frequency array
    for ( int i = 0; i < 10; i++ )
        freq[i] = 0;

	printf("Cactus Kev's Hand Evaluator, by Kevin Suffecool\n");
	printf("-----------------------------------------------\n\n");
	printf("Enumerating and evaluating all 2,598,960 unique five-card poker hands...\n\n");

	DWORD dwTime = GetTickCount();

	//////////////////////////////////////////////////////////////////////////////
	// Now let's enumerate and evaluate all 5-card poker hands...
	//////////////////////////////////////////////////////////////////////////////

    for(c0 = 0; c0 < 48; c0++) {
		hand[0] = deck[c0];
		for(c1 = c0 + 1; c1 < 49; c1++) {
			hand[1] = deck[c1];
			for(c2 = c1 + 1; c2 < 50; c2++) {
				hand[2] = deck[c2];
				for(c3 = c2 + 1; c3 < 51; c3++) {
					hand[3] = deck[c3];
					for(c4 = c3 + 1; c4 < 52; c4++) {
						hand[4] = deck[c4];

						equivClass = eval_5hand( hand );
						handCategory = hand_rank( equivClass );
						freq[handCategory]++;
					}
				}
			}
		}
    }

	dwTime = GetTickCount() - dwTime;

    for(int i=1;i<=9;i++)
		printf( "%15s: %8d\n", value_str[i], freq[i] );

	int totalHands = 0;
	for (int category = 0; category < 10; category++)
		totalHands += freq[category];
	printf("\nEnumerated and evaluated %d hands in %d milliseconds!\n\n", totalHands, dwTime);

	//////////////////////////////////////////////////////////////////////////////
	// Now let's enumerate and evaluate all 7-card poker hands...
	//////////////////////////////////////////////////////////////////////////////

	memset(freq, 0, sizeof(freq));
	printf("Enumerating and evaluating all 133,784,560 possible 7-card poker hands...\n");
	printf("This will take a few minutes...\n\n");

	dwTime = GetTickCount();

	for (c0 = 0; c0 < 46; c0++) {
		hand[0] = deck[c0];
		for (c1 = c0+1; c1 < 47; c1++) {
			hand[1] = deck[c1];
 			for (c2 = c1+1; c2 < 48; c2++) {
				hand[2] = deck[c2];
				for (c3 = c2+1; c3 < 49; c3++) {
					hand[3] = deck[c3];
 					for (c4 = c3+1; c4 < 50; c4++) {
						hand[4] = deck[c4];
						for (c5 = c4+1; c5 < 51; c5++) {
							hand[5] = deck[c5];
 							for (c6 = c5+1; c6 < 52; c6++) {
								hand[6] = deck[c6];

								equivClass = eval_7hand( hand );
								handCategory = hand_rank( equivClass );
								freq[handCategory]++;
							}
						}
					}
				}
			}
		}
	}

	dwTime = GetTickCount() - dwTime;

   for(int i=1;i<=9;i++)
		printf( "%15s: %8d\n", value_str[i], freq[i] );

	totalHands = 0;
	for (int category = 0; category < 10; category++)
		totalHands += freq[category];
	printf("\nEnumerated and evaluated %d hands in %d milliseconds!\n\n", totalHands, dwTime);

	std::cin.get();
}