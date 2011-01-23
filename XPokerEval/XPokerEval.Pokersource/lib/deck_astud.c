/*
 *  Copyright 2006 Michael Maurer <mjmaurer@yahoo.com>, 
 *                 Brian Goetz <brian@quiotix.com>, 
 *                 Loic Dachary <loic@dachary.org>, 
 *                 Tim Showalter <tjs@psaux.com>
 *
 *  This package is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 dated June, 1991.
 *
 *  This package is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this package; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 */
#include <stdio.h>
#include <ctype.h>
#include "poker_defs.h"
#include "deck_astud.h"

const char AStudDeck_rankChars[] = "%%%%%789TJQKA";
const char AStudDeck_suitChars[] = "hdcs";

int 
AStudDeck_cardToString(int cardIndex, char *outString) {
  *outString++ = AStudDeck_rankChars[AStudDeck_RANK(cardIndex)];
  *outString++ = AStudDeck_suitChars[AStudDeck_SUIT(cardIndex)];
  *outString   = '\0';

  return 2;
}


int 
AStudDeck_stringToCard(char *inString, int *cardIndex) {
  char *p;
  int rank, suit;

  p = inString;
  for (rank=AStudDeck_Rank_FIRST; rank <= AStudDeck_Rank_LAST; rank++) 
    if (AStudDeck_rankChars[rank] == toupper(*p))
      break;
  if (rank > AStudDeck_Rank_LAST)
    goto noMatch;
  ++p;
  for (suit=AStudDeck_Suit_FIRST; suit <= AStudDeck_Suit_LAST; suit++) 
    if (AStudDeck_suitChars[suit] == tolower(*p))
      break;
  if (suit > AStudDeck_Suit_LAST)
    goto noMatch;
  *cardIndex = AStudDeck_MAKE_CARD(rank, suit);
  return 2;

 noMatch:
  /* Didn't match anything, return failure */
  return 0;
}


int
AStudDeck_maskToCards(void *cardMask, int cards[]) {
  int i, n=0;
  AStudDeck_CardMask c = *((AStudDeck_CardMask *) cardMask);

  for (i=AStudDeck_N_CARDS-1; i >= 0; i--) 
    if (AStudDeck_CardMask_CARD_IS_SET(c, i)) 
      cards[n++] = i;

  return n;
}

int 
AStudDeck_NumCards(void *cardMask) {
  AStudDeck_CardMask c = *((AStudDeck_CardMask *) cardMask);
  int i;
  int ncards = 0;
  for (i=0; i<AStudDeck_N_CARDS; i++)
    if (AStudDeck_CardMask_CARD_IS_SET(c, i))
      ncards++;
  return ncards;
}

Deck AStudDeck = { 
  AStudDeck_N_CARDS, 
  "AsianStudDeck", 
  AStudDeck_cardToString, 
  AStudDeck_stringToCard,
  AStudDeck_maskToCards,
  AStudDeck_NumCards
};
