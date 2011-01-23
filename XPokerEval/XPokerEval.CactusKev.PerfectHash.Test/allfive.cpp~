#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include <windows.h>
#include "..\XPokerEval.CactusKev.PerfectHash\poker.h"

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
/*************************************************/

int _tmain(int argc, _TCHAR* argv[])
{
    int deck[52], hand[5], freq[10];
    int a, b, c, d, e, i, j;

    // initialize the deck
    init_deck( deck );

    // zero out the frequency array
    for ( i = 0; i < 10; i++ )
        freq[i] = 0;

	printf("Cactus Kev's Hand Evaluator with Perfect Hash by Paul Senzee\n");
	printf("------------------------------------------------------------\n\n");
	printf("Enumerating and evaluating all 2,598,960 unique five-card poker hands...\n\n");

	DWORD dwTime = GetTickCount();

    // loop over every possible five-card hand
    for(a=0;a<48;a++)
    {
		hand[0] = deck[a];
		for(b=a+1;b<49;b++)
		{
			hand[1] = deck[b];
			for(c=b+1;c<50;c++)
			{
				hand[2] = deck[c];
				for(d=c+1;d<51;d++)
				{
					hand[3] = deck[d];
					for(e=d+1;e<52;e++)
					{
						hand[4] = deck[e];

						i = eval_5hand( hand );
						j = hand_rank(i);
						freq[j]++;
					}
				}
			}
		}
    }

	dwTime = GetTickCount() - dwTime;

    for(i=1;i<=9;i++)
		printf( "%15s: %8d\n", value_str[i], freq[i] );

	int totalHands = 0;
	for (int category = 0; category < 10; category++)
		totalHands += freq[category];
	printf("\nEnumerated and evaluated %d hands in %d milliseconds!", totalHands, dwTime);

	std::cin.get();
}

