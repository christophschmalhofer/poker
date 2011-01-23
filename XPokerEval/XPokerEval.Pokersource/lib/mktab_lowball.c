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
#include "mktable.h"
#include "handval_low.h"

#define B5C_COMMENT_STRING \
 "bottomFiveCardsTable[].  Maps 13-bit rank masks to a HandVal with\n" \
 "the xx_card fields representing the low five bits set.  \n" \
 "If there are not five unique bits set, value is\n" \
 "zero.  Treats 2 as low and A as high bits; to do A-5 low evaluation, \n" \
 "you will need to rotate the input mask.\n"  
#define B5C_FILENAME "t_botfivecards"

#define B5J_COMMENT_STRING \
 "bottomFiveJokerTable[].  Maps 13-bit rank masks to a HandVal with\n" \
 "the xx_card fields representing the low five bits set, assuming there.  \n" \
 "is a joker in the hand. If there are not five unique bits set, value is\n" \
 "zero.  Treats 2 as low and A as high bits; to do A-5 low evaluation, \n" \
 "you will need to rotate the input mask.\n"  
#define B5J_FILENAME "t_botfivecardsj"

#define BC_COMMENT_STRING \
 "bottomCardTable[].  Maps 13-bit rank masks to a number identifying the\n" \
 "low bit that is set.  If the input has no bits set, the answer is not\n" \
 "meaningful. \n" \
 "Treats 2 as low and A as high bits; to do A-5 low evaluation, \n" \
 "you will need to rotate the input mask.\n"  
#define BC_FILENAME "t_botcard"

static void 
doBottomFiveCardsTable(void) {
  int i, j, card;

  MakeTable_begin("bottomFiveCardsTable", 
                  B5C_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(B5C_COMMENT_STRING);
  MakeTable_comment("If there are not five unique ranks, returns zero");
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    LowHandVal eval;
    int n = i;

    eval = 0;
    for (j=0; j<5; j++) {
      if (n == 0) {
        eval = 0;
        break;
      };
      card = bottom_card_func(n);
      eval += (card << j*HandVal_CARD_WIDTH);
      n &= ~(1 << card);
    };

    MakeTable_outputUInt32(eval);
  };

  MakeTable_end();
}


static void 
doBottomFiveJokerTable(void) {
  int i, j, card;

  MakeTable_begin("bottomFiveJokerTable", 
                  B5J_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(B5J_COMMENT_STRING);
  MakeTable_comment("If there are not five unique ranks, returns zero");
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    LowHandVal eval;
    int n = i;

    eval = 0;

    /* Set the bottommost unset bit */
    for (j=0; j<StdDeck_Rank_COUNT; j++) 
      if (!(i & (1 << j))) {
        i |= (1 << j);
        break;
      };
    
    for (j=0; j<5; j++) {
      if (n == 0) {
        eval = 0;
        break;
      };
      eval <<= HandVal_CARD_WIDTH;
      card = bottom_card_func(n);
      eval += card;
      n &= ~(1 << card);
    };

    MakeTable_outputUInt32(eval);
  };

  MakeTable_end();
}


static void 
doBottomCardTable(void) {
  int i;

  MakeTable_begin("bottomCardTable", 
                  BC_FILENAME, 
                  "uint8", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(BC_COMMENT_STRING);
  MakeTable_comment("Value is only meaningful if there is actually a bit set");
  for (i=0; i < StdDeck_N_RANKMASKS; i++) 
    MakeTable_outputUInt8(bottom_card_func(i));

  MakeTable_end();
}


int 
main(int argc, char **argv) {
  doBottomFiveCardsTable();
  doBottomFiveJokerTable();
  doBottomCardTable();
  return 0;
}

