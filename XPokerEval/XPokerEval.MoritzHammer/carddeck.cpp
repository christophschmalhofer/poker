#include "carddeck.h"
#include "randomgenerator.h"
#include <string.h>

char unique(deck_struct* deck) {
	unsigned i, j;
	for (i=0; i < 52; i++) {
		for (j = i+1; j < 52; j++) {
			assert(deck->cards[i] != deck->cards[j]);
		}
	}
	return 1;
}

void init_deck(deck_struct* deck) {
	unsigned int i;
	for (i=0; i < 52; i++) {
		deck->cards[i] = (char) i;		
	}
	deck->remaining = 52;
}

char draw_card(deck_struct* deck, const char idx) {
	char swap = deck->cards[(int) idx];
	deck->remaining--;
	deck->cards[(int) idx] = deck->cards[deck->remaining];
	deck->cards[deck->remaining] = swap;
	return swap;
}

void remove_card(deck_struct* deck, const char card) {
	// guess that the card is at the proper position			
	char idx = card;
	while (1) {
		if (deck->cards[(int) idx] == card) {
			draw_card(deck, idx);
			return; 
		}
		idx = (idx + 1) % deck->remaining;
	}
}

char parse_card(char* str) {
	if (strlen(str) != 2) {
		printf("Invalid card string %s\n", str);
		exit(1);
	}
	char r = 0;
	unsigned int i, f = 0;
	for (i=0; i < 13; i++) {
		if (str[0] == CARD[i]) {
			f++;
			r = i << 2;
		}
	}
	for (i=0; i < 4; i++) {
		if (str[1] == COLOR[i]) {
			f++;
			r += i;
		}
	}
	if (f != 2) {
		printf("Invalid card string %s\n", str);
		exit(1);
	}
	return r;
}

char _deckswap;


char draw_random_card(deck_struct *deck) {
	char idx = get_random_card(deck->remaining);	
	return draw_card(deck, idx);
}

void reset_deck(deck_struct *deck) {
	deck->remaining = 52;
}

