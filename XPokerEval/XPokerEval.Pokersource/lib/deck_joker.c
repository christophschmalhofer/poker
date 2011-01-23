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
#include "deck_joker.h"

const char JokerDeck_rankChars[] = "23456789TJQKA";
const char JokerDeck_suitChars[] = "hdcs";

/*
 * These implementations assume that the card indices and suit bit fields
 * between StdDeck and JokerDeck are the same!
 */

int 
JokerDeck_cardToString(int cardIndex, char *outString) {
  if (JokerDeck_IS_JOKER(cardIndex)) {
    *outString++ = 'X';
    *outString++ = 'x';
    *outString   = '\0';
    return 2;
  }
  else 
    return StdDeck_cardToString(cardIndex, outString);
}


int
JokerDeck_maskToCards(void *cardMask, int cards[]) {
  int i, n=0;
  JokerDeck_CardMask c = *((JokerDeck_CardMask *) cardMask);

  for (i=JokerDeck_N_CARDS-1; i >= 0; i--) 
    if (JokerDeck_CardMask_CARD_IS_SET(c, i)) 
      cards[n++] = i;

  return n;
}


int 
JokerDeck_stringToCard(char *inString, int *cardIndex) {

  if ((toupper(inString[0]) == 'X') && toupper(inString[1]) == 'X') {
    *cardIndex = JokerDeck_JOKER;
    return 2;
  }
  else 
    return StdDeck_stringToCard(inString, cardIndex);
}

int 
JokerDeck_NumCards(void *cardMask) {
  JokerDeck_CardMask c = *((JokerDeck_CardMask *) cardMask);
  int i;
  int ncards = 0;
  for (i=0; i<JokerDeck_N_CARDS; i++)
    if (JokerDeck_CardMask_CARD_IS_SET(c, i))
      ncards++;
  return ncards;
}

Deck JokerDeck = { 
  JokerDeck_N_CARDS, 
  "JokerDeck", 
  JokerDeck_cardToString, 
  JokerDeck_stringToCard,
  JokerDeck_maskToCards,
  JokerDeck_NumCards
};
