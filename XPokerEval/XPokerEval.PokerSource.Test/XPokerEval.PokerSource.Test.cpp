#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <iostream>
#include <windows.h>
#include "..\XPokerEval.PokerSource\include\inlines\eval.h"
#include "..\XPokerEval.PokerSource\include\inlines\eval_type.h"

// Helper:
// Given a string such as "AcKcQcJcTc" representing a poker hand,
// return an StdDeck_CardMask representing that hand.
StdDeck_CardMask handStringToMask(char* strHand)
{
	StdDeck_CardMask mask;
	StdDeck_CardMask_RESET(mask);

	int cardIndex = -1;
	int numCards = strlen(strHand) / 2;
	for (int cur = 0; cur < numCards; cur++)
	{
		char* pCard = &strHand[cur * 2];
		StdDeck_stringToCard(pCard, &cardIndex);
		mask.cards_n |= StdDeck_MASK(cardIndex).cards_n;
	}

	return mask;
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("Testing the Pokersource Poker-Eval evaluator\n");
	printf("--------------------------------------------\n\n");
	printf("Initializing the Pokersource Poker-Eval card deck...\n\n");

	// Initialize a deck of cards in the Pokersource format...
	// Not strictly necessary to do this exactly this way but...
	StdDeck_CardMask virginDeck[52];
    for (int index = 0; index < 52; index++)
	{
		printf("Card #%d: %c%c\n", index, 
			StdDeck_rankChars[ StdDeck_RANK(index) ], 
			StdDeck_suitChars[ StdDeck_SUIT(index) ]);
        virginDeck[index] = StdDeck_MASK(index);
	}

	printf("\nEnumerating and evaluating all 133,784,560 possible 7-card poker hands...\n\n");

	int count = 0;
	int handTypeSum[10];
	int c0, c1, c2, c3, c4, c5, c6;
	memset(handTypeSum, 0, sizeof(handTypeSum));  // do init..
	// Here we'll use one card mask per card, so that the loops below
	// can store partial hand state in the outer iterations of the loop,
	// saving a little time. We could have as well done this with a
	// single StdDeck_CardMask however.
    StdDeck_CardMask h0, h1, h2, h3, h4, h5, h6;


	DWORD dwTime = GetTickCount();

	// Iterate across all 7-card poker hands...
	for (c0 = 0; c0 < 46; c0++) {
		h0 = virginDeck[c0]; // first card
		for (c1 = c0+1; c1 < 47; c1++) {
			h1.cards_n = h0.cards_n | virginDeck[c1].cards_n; // 2nd card
 			for (c2 = c1+1; c2 < 48; c2++) {
				h2.cards_n = h1.cards_n | virginDeck[c2].cards_n; // 3rd card
				for (c3 = c2+1; c3 < 49; c3++) {
					h3.cards_n = h2.cards_n | virginDeck[c3].cards_n; // 4th card
 					for (c4 = c3+1; c4 < 50; c4++) {
						h4.cards_n = h3.cards_n | virginDeck[c4].cards_n; // 5th card
						for (c5 = c4+1; c5 < 51; c5++) {
							h5.cards_n = h4.cards_n | virginDeck[c5].cards_n; // 6th card
 							for (c6 = c5+1; c6 < 52; c6++) {
                                h6.cards_n = h5.cards_n | virginDeck[c6].cards_n; // 7th card

								// Okay, we can evaluate the hand one of two ways:
								//    1. By using the EVAL_N evaluator and taking 
								//       the returned HandVal and rxtracting the
								//       category with HandVal_HANDTYPE, OR...
								//    2. By using the EVAL_TYPE evaluator, which
								//       returns the hand type and is faster.

								//handTypeSum[HandVal_HANDTYPE(StdDeck_StdRules_EVAL_N(h6, 7))+1]++;
								handTypeSum[StdDeck_StdRules_EVAL_TYPE(h6, 7) + 1]++;

								count++;
							}
						}
					}
				}
			}
		}
	}

	dwTime = GetTickCount() - dwTime;

	printf("BAD:              %d\n", handTypeSum[0]);
	printf("High Card:        %d\n", handTypeSum[1]);
	printf("One Pair:         %d\n", handTypeSum[2]);
	printf("Two Pair:         %d\n", handTypeSum[3]);
	printf("Trips:            %d\n", handTypeSum[4]);
	printf("Straight:         %d\n", handTypeSum[5]);
	printf("Flush:            %d\n", handTypeSum[6]);
	printf("Full House:       %d\n", handTypeSum[7]);	
	printf("Quads:            %d\n", handTypeSum[8]);
	printf("Straight Flush:   %d\n", handTypeSum[9]);

	printf("\nEnumerated and evaluated %d hands in %d milliseconds!\n\n", count, dwTime);

	// We can convert card text such as "As" or "2d" to an index (0-51):
	int cardIndex = 0;
	StdDeck_stringToCard("As", &cardIndex);

	// And we can convert a card Index to a StdDeck_CardMask value
	StdDeck_CardMask aceOfSpades = StdDeck_MASK(cardIndex);

	// But let's create a new poker hand...
	StdDeck_CardMask myHand;
	// Make sure the hand is empty by setting all bits to 0
	StdDeck_CardMask_RESET(myHand);

	// We can combine two or more StdDeck_CardMask as follows
	// (this is how we "add cards" to a hand)
	StdDeck_CardMask_OR(myHand, myHand, aceOfSpades);
	// Which is equivalent to:
	myHand.cards_n |= aceOfSpades.cards_n;

	// Now let's create a royal flush of spades and of hearts
	StdDeck_CardMask royalSpades = handStringToMask("AsKsQsJsTs");
	StdDeck_CardMask royalHearts = handStringToMask("AhKhQhJhTh");

	// Now let's use the N-card evaluator to get the hand value
	// We can compare these values to determine the better hand
	// These two should tie:
	HandVal hv1 = StdDeck_StdRules_EVAL_N(royalSpades, 5);
	HandVal hv2 = StdDeck_StdRules_EVAL_N(royalHearts, 5);
	if (hv1 != hv2)
		printf("Error!\nError!\nError!\n");
	
	// Meaning, that their hand values are the same:
	printf("Hand Value for Royal Flush of Spades = %d, Hearts = %d\n", hv1, hv2);

	// But now let's compare two unequal 7-card hands...Quad 8's and 2 pair
	StdDeck_CardMask quad8s = handStringToMask("4d8c8s8h8dAsAd");
	StdDeck_CardMask twoPair = handStringToMask("AsKd2cAhKc7hQd");

	// Use the EVAL_N evaluator for both hands..notice we pass "7" in 
	// now as we want to evaluate a 7-card hand
	hv1 = StdDeck_StdRules_EVAL_N(quad8s, 7);
	hv2 = StdDeck_StdRules_EVAL_N(twoPair, 7);
	if (hv1 <= hv2)
		printf("Error!\nError!\nError!\n");

	// There's also a (quicker) N-card "type evaluator". All it does is
	// retrieve the hand category (no hand value)
	int nCat1 = StdDeck_StdRules_EVAL_TYPE(royalSpades, 5);
	int nCat2 = StdDeck_StdRules_EVAL_TYPE(royalHearts, 5);
	if (nCat1 != nCat2 || nCat1 != StdRules_HandType_STFLUSH)
		printf("Error!\nError!\nError!\n");

	// But let's say all you have is a HandVal and you want to get the hand type?
	// Use HandVal_HANDTYPE(handvalue):
	int ht1 = HandVal_HANDTYPE(hv1);

	printf("\nDone! Press Enter to continue...");
	std::cin.get();

	return 0;
}

