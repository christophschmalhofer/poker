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

#include "poker_defs.h"
#include "deck_joker.h"
#include "mktable.h"

#define JCM_COMMENT_STRING \
 "JokerDeck_cardMasks[].  Maps card indices (0..53) to CardMasks.  \n"       \
 "The output mask has only one bit set, the bit corresponding to the card\n" \
 "identified by the index." 
#define JCM_FILENAME "t_jokercardmasks"

#define JST_COMMENT_STRING \
 "straightTable[].  Maps 13-bit rank masks to a value indicating if a \n"    \
 "straight would be present if we had a joker, and if so, the rank of \n"    \
 "the high card.  Zero means no straight, even though zero corresponds\n"    \
 "to deuce, but since there is no such thing as a deuce-high straight, \n"  \
 "that's OK. "
#define JST_FILENAME "t_jokerstraight"



static void 
doCardMaskTable(void) {
  JokerDeck_CardMask c;
  int i;

  MakeTable_begin("JokerDeck_cardMasksTable", 
                  JCM_FILENAME, 
                  "JokerDeck_CardMask", 
                  JokerDeck_N_CARDS);
  MakeTable_comment(JCM_COMMENT_STRING);
  MakeTable_extraCode("#include \"deck_joker.h\"");
  for (i=0; i<JokerDeck_N_CARDS; i++) {
    JokerDeck_CardMask_RESET(c);
    if (JokerDeck_IS_JOKER(i))
      c.cards.joker = 1;
    else {
      int suit = JokerDeck_SUIT(i);
      int rank = JokerDeck_RANK(i);
      if (suit == JokerDeck_Suit_HEARTS)
        c.cards.hearts = (1 << rank);
      else if (suit == JokerDeck_Suit_DIAMONDS)
        c.cards.diamonds = (1 << rank);
      else if (suit == JokerDeck_Suit_CLUBS)
        c.cards.clubs = (1 << rank);
      else if (suit == JokerDeck_Suit_SPADES)
        c.cards.spades = (1 << rank);
    };

#ifdef USE_INT64
    MakeTable_outputUInt64(c.cards_n);
#else
    {
      char buf[80];
      sprintf(buf, " { { 0x%08x, 0x%08x } } ", c.cards_nn.n1, c.cards_nn.n2);
      MakeTable_outputString(buf);
    };
#endif
  };

  MakeTable_end();
}


static void 
doStraightTable(void) {
  int i, j;

  MakeTable_begin("jokerStraightTable", 
                  JST_FILENAME, 
                  "uint8", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(JST_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    int maxSf = 0, sf;
    for (j=StdDeck_Rank_FIRST; j <= StdDeck_Rank_LAST; j++) {
      sf = straight_func(i | (1 << j));
      if (sf > maxSf) 
        maxSf = sf;
    };
    
    MakeTable_outputUInt8(maxSf);
  };

  MakeTable_end();
}




int 
main(int argc, char **argv) {
  doCardMaskTable();
  doStraightTable();

  return 0;
}

