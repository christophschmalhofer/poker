#ifndef __CARDDECK_H__
#define __CARDDECK_H__
#include "common.h"

typedef struct deck_struct {
	char cards[52];
	unsigned int remaining;
} deck_struct;

char unique(deck_struct* deck);
void init_deck(deck_struct* deck);
char draw_card(deck_struct* deck, const char idx);
void remove_card(deck_struct* deck, const char card);
char parse_card(char* str);
char draw_random_card(deck_struct *deck);
void reset_deck(deck_struct *deck);

#endif
