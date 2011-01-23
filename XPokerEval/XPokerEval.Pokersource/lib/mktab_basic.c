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

#define CM_COMMENT_STRING \
 "StdDeck_cardMasks[].  Maps card indices (0..51) to CardMasks.  \n"         \
 "The output mask has only one bit set, the bit corresponding to the card\n" \
 "identified by the index." 
#define CM_FILENAME "t_cardmasks"

#define TB_COMMENT_STRING \
 "topBitTable[].  Maps 13-bit rank masks to a 13-bit rank mask.\n"           \
 "The output mask has only one bit set, the bit corresponding to the\n"      \
 "highest bit which was set in the input mask. "
#define TB_FILENAME "t_topbit"

#define TC_COMMENT_STRING \
 "topCardTable[].  Maps 13-bit rank masks to an integer corresponding to\n"  \
 "a card rank.  The output value is the rank of the highest card set \n"     \
 "in the input mask.  "
#define TC_FILENAME "t_topcard"

#define T5C_COMMENT_STRING \
 "topFiveCardsTable[].  Maps 13-bit rank masks to a HandVal structure with\n" \
 "the xx_card fields set.  The fields correspond to the rank values of the\n" \
 "top five bits set in the input rank mask. "
#define T5C_FILENAME "t_topfivecards"

#define T5B_COMMENT_STRING \
 "topFiveBitsTable[].  Maps 13-bit rank masks to a rank mask with the \n" \
 "the five bits set which were the top five bits set in the input rank mask."
#define T5B_FILENAME "t_topfivebits"

#define T2B_COMMENT_STRING \
 "topFiveBitsTable[].  Maps 13-bit rank masks to a rank mask with the \n" \
 "the two bits set which were the top two bits set in the input rank mask."
#define T2B_FILENAME "t_toptwobits"

#define NB_COMMENT_STRING \
 "nBitsTable[].  Maps 13-bit rank masks to the number of bits that are set\n" \
 "in the mask. "
#define NB_FILENAME "t_nbits"

#define ST_COMMENT_STRING \
 "straightTable[].  Maps 13-bit rank masks to a value indicating if a \n"    \
 "straight is present, and if so, the rank of the high card.  Zero means\n"  \
 "no straight, even though zero corresponds to deuce, but since there\n"     \
 "is no such thing as a deuce-high straight, that's OK. "
#define ST_FILENAME "t_straight"

#define MR_COMMENT_STRING \
 "StdDeck_masksRanksTable[].  Maps card ranks (2..A) to a cardmask which\n"   \
 "has all the bits set except the bits corresponding to the cards whose\n"   \
 "have the input rank.  This is a quick way to mask off all the cards of\n"  \
 "a specific rank."
#define MR_FILENAME "t_maskrank"



static void 
doNBitsTable(void) {
  int i;

  MakeTable_begin("nBitsTable", 
                  NB_FILENAME, 
                  "uint8", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(NB_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) 
    MakeTable_outputUInt8(n_bits_func(i));

  MakeTable_end();
}


static void 
doTopCardTable(void) {
  int i;

  MakeTable_begin("topCardTable", 
                  TC_FILENAME, 
                  "uint8", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(TC_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) 
    MakeTable_outputUInt8(top_card_func(i));

  MakeTable_end();
}


static void 
doTopBitTable(void) {
  int i;

  MakeTable_begin("topBitTable", 
                  TB_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(TB_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) 
    MakeTable_outputUInt32(top_bit_func(i));

  MakeTable_end();
}


static void 
doTopFiveCardsTable(void) {
  int i, j, card;

  MakeTable_begin("topFiveCardsTable", 
                  T5C_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(T5C_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    HandVal eval;
    int n = i;

    eval = 0;
    for (j=0; j<5; j++) {
      eval <<= HandVal_CARD_WIDTH;
      card = top_card_func(n);
      eval += card;
      n &= ~(1 << card);
    };

    MakeTable_outputUInt32(eval);
  };

  MakeTable_end();
}


static void 
doTopFiveBitsTable(void) {
  int i;

  MakeTable_begin("topFiveBitsTable", 
                  T5B_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(T5B_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    uint32 retval, temp, n;

    n = i;
    temp = top_bit_func( n );  retval  = temp;  n &= ~temp;
    temp = top_bit_func( n );  retval |= temp;  n &= ~temp;
    temp = top_bit_func( n );  retval |= temp;  n &= ~temp;
    temp = top_bit_func( n );  retval |= temp;  n &= ~temp;
    temp = top_bit_func( n );  retval |= temp;

    MakeTable_outputUInt32(retval);
  };

  MakeTable_end();
}


static void 
doTopTwoBitsTable(void) {
  int i;

  MakeTable_begin("topTwoBitsTable", 
                  T2B_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(T2B_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    uint32 retval, n;

    n = i;
    retval = top_bit_func(n);
    n ^= retval;
    retval |= top_bit_func(n);
    MakeTable_outputUInt32(retval);
  };

  MakeTable_end();
}


static void 
doStraightTable(void) {
  int i;

  MakeTable_begin("straightTable", 
                  ST_FILENAME, 
                  "uint8", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(ST_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) 
    MakeTable_outputUInt8(straight_func(i));

  MakeTable_end();
}


static void 
doCardMaskTable(void) {
  StdDeck_CardMask c;
  int i;

  MakeTable_begin("StdDeck_cardMasksTable", 
                  CM_FILENAME, 
                  "StdDeck_CardMask", 
                  StdDeck_N_CARDS);
  MakeTable_comment(CM_COMMENT_STRING);
  for (i=0; i<StdDeck_N_CARDS; i++) {
    int suit = StdDeck_SUIT(i);
    int rank = StdDeck_RANK(i);

    StdDeck_CardMask_RESET(c);
    if (suit == StdDeck_Suit_HEARTS)
      c.cards.hearts = (1 << rank);
    else if (suit == StdDeck_Suit_DIAMONDS)
      c.cards.diamonds = (1 << rank);
    else if (suit == StdDeck_Suit_CLUBS)
      c.cards.clubs = (1 << rank);
    else if (suit == StdDeck_Suit_SPADES)
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


static void 
doMaskRankTable(void) {
  int i;

  MakeTable_begin("StdDeck_maskRankTable", 
                  MR_FILENAME, 
                  "StdDeck_CardMask", 
                  StdDeck_Rank_COUNT);
  MakeTable_comment(MR_COMMENT_STRING);
  for (i=0; i < StdDeck_Rank_COUNT; i++) {
    StdDeck_CardMask c;

    StdDeck_CardMask_RESET(c);
    c.cards.spades   = (1 << i);
    c.cards.hearts   = (1 << i);
    c.cards.diamonds = (1 << i);
    c.cards.clubs    = (1 << i);

#ifdef USE_INT64
    MakeTable_outputUInt64(~c.cards_n);
#else
    { 
      char buf[80];
      sprintf(buf, " { { 0x%08x, 0x%08x } } ", ~c.cards_nn.n1, ~c.cards_nn.n2);
      MakeTable_outputString(buf);
    };
#endif
  };

  MakeTable_end();
}


int 
main(int argc, char **argv) {
  doCardMaskTable();
  doMaskRankTable();
  doNBitsTable();
  doTopCardTable();
  doTopBitTable();
  doTopFiveCardsTable();
  doTopFiveBitsTable();
  doTopTwoBitsTable();
  doStraightTable();

  return 0;
}

