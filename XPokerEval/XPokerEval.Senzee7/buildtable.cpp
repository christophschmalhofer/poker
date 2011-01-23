#include "..\XPokerEval.PokerSource\include\inlines\eval.h"
#include "..\XPokerEval.PokerSource\include\inlines\eval_type.h"
#include "index52c7.h"
#include "..\XPokerEval.CactusKev.PerfectHash\poker.h"
#include <string.h> // for memset

#define LOOKUP_TABLE_SIZE 133784560

void destroy_table(short int* theTable)
{
	delete [] theTable;
}

int cactus_primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 };

int DeckIndexToCactusKevCard(int nCardIndex)
{
	int suit = StdDeck_SUIT(nCardIndex);
	int rank = StdDeck_RANK(nCardIndex);

	// Pokersource uses different suit format than Cactus Kev..
	switch(suit)
	{
	case StdDeck_Suit_HEARTS: suit = HEART; break;
	case StdDeck_Suit_CLUBS: suit = CLUB; break;
	case StdDeck_Suit_SPADES: suit = SPADE; break;
	case StdDeck_Suit_DIAMONDS: suit = DIAMOND; break;
	}

	// Build the Cactus Kev card
	return cactus_primes[rank] | (rank << 8) | suit | (1 << (16 + rank));
}

short int* create_table(const char* saveAs)
{
	printf("Allocating memory for 133,784,560 entries...\n");
	short int* hand_lookup = new short int[LOOKUP_TABLE_SIZE];
	if (!hand_lookup)
		return NULL;

	// Load the HandRanks.DAT file and map it into the HR array
	printf("Attempting to load %s lookup table...\n", saveAs);
	memset(hand_lookup, 0, LOOKUP_TABLE_SIZE * 2);
	FILE * fin = fopen(saveAs, "rb");
	if (fin)
	{
		size_t bytesread = fread(hand_lookup, 2, LOOKUP_TABLE_SIZE, fin);
		fclose(fin);
		printf("Successfully loaded %s!\n", saveAs);
		return hand_lookup;
	}

	printf("%s file not found. Creating it...\n", saveAs);

	printf("Initializing decks...\n");
	// Initialize the Pokersource deck...
	__int64 virginDeck[52];
    for (int c = 0; c < 52; c++)
        virginDeck[c] = (0x1LL << c);

	// Initialize the Cactus Kev deck...
	int* cactusKevDeck = new int[52];
	init_deck(cactusKevDeck);

	int c0, c1, c2, c3, c4, c5, c6;
	__int64 h0, h1, h2, h3, h4, h5, h6;
	int cactuskev_hand[7];
	int count = 0;

	printf("Building table: enumerating 7-card hands...\n");
	for (c0 = 0; c0 < 46; c0++) {
		h0 = virginDeck[c0]; // first card
		cactuskev_hand[0] = DeckIndexToCactusKevCard(c0);
		for (c1 = c0+1; c1 < 47; c1++) {
			h1 = h0 | virginDeck[c1]; // 2nd card
			cactuskev_hand[1] = DeckIndexToCactusKevCard(c1);
 			for (c2 = c1+1; c2 < 48; c2++) {
				h2 = h1 | virginDeck[c2]; // 3rd card
				cactuskev_hand[2] = DeckIndexToCactusKevCard(c2);
				for (c3 = c2+1; c3 < 49; c3++) {
					h3 = h2 | virginDeck[c3]; // 4th card
					cactuskev_hand[3] = DeckIndexToCactusKevCard(c3);
 					for (c4 = c3+1; c4 < 50; c4++) {
						h4 = h3 | virginDeck[c4]; // 5th card
						cactuskev_hand[4] = DeckIndexToCactusKevCard(c4);
						for (c5 = c4+1; c5 < 51; c5++) {
							h5 = h4 | virginDeck[c5]; // 6th card
							cactuskev_hand[5] = DeckIndexToCactusKevCard(c5);
 							for (c6 = c5+1; c6 < 52; c6++) {
                                h6 = h5 | virginDeck[c6]; // 7th card
								cactuskev_hand[6] = DeckIndexToCactusKevCard(c6);

								int hashedIndex = index52c7(h6);
								hand_lookup[hashedIndex] = eval_7hand(cactuskev_hand);

								if ((count++ % 1000000) == 0)
									printf("%d hands complete...\n", count);
							}
						}
					}
				}
			}
		}
	}

	printf("Saving the lookup table into %s...\n", saveAs);

	// output the array now that I have it!!
	FILE * fout = fopen(saveAs, "wb");
	if (fout)
	{
		fwrite(hand_lookup, 2, LOOKUP_TABLE_SIZE, fout);  // big write, but quick
		fclose(fout);
	}
	else
	{
		printf("Problem creating the Output File!\n");
	}

	printf("Press any key to continue...");
	
	return hand_lookup;
}