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
#include "deck_astud.h"
#include "mktable.h"

#define ACM_COMMENT_STRING \
 "AStudDeck_cardMasks[].  Maps card indices (0..32) to CardMasks.  \n"       \
 "The output mask has only one bit set, the bit corresponding to the card\n" \
 "identified by the index." 
#define ACM_FILENAME "t_astudcardmasks"



static void 
doCardMaskTable(void) {
  StdDeck_CardMask c;
  int i;

  MakeTable_begin("AStudDeck_cardMasksTable", 
                  ACM_FILENAME, 
                  "AStudDeck_CardMask", 
                  AStudDeck_N_CARDS);
  MakeTable_comment(ACM_COMMENT_STRING);
  MakeTable_extraCode("#include \"deck_astud.h\"");
  for (i=0; i<AStudDeck_N_CARDS; i++) {
    int suit = AStudDeck_SUIT(i);
    int rank = AStudDeck_RANK(i);

    AStudDeck_CardMask_RESET(c);
    if (suit == AStudDeck_Suit_HEARTS)
      c.cards.hearts = (1 << rank);
    else if (suit == AStudDeck_Suit_DIAMONDS)
      c.cards.diamonds = (1 << rank);
    else if (suit == AStudDeck_Suit_CLUBS)
      c.cards.clubs = (1 << rank);
    else if (suit == AStudDeck_Suit_SPADES)
      c.cards.spades = (1 << rank);

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


int 
main(int argc, char **argv) {
  doCardMaskTable();

  return 0;
}

