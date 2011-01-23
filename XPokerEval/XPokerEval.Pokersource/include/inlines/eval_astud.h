/*
 *  eval_astud.h: a fast poker hand evaluator for asian stud
 *
 *  Copyright (C) 1999 Brian Goetz
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

#ifndef __EVAL_ASTUD_H__
#define __EVAL_ASTUD_H__

#include "poker_defs.h"
#include "inlines/eval.h"

#define SC AStudDeck_CardMask_CLUBS(cards)
#define SD AStudDeck_CardMask_DIAMONDS(cards)
#define SH AStudDeck_CardMask_HEARTS(cards)
#define SS AStudDeck_CardMask_SPADES(cards)

static uint32 astudHandTypeMap[StdRules_HandType_LAST+1] = {
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_NOPAIR), 
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_ONEPAIR), 
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_TWOPAIR), 
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_TRIPS), 
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_STRAIGHT), 
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_FLUSH), 
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_FULLHOUSE), 
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_QUADS), 
  HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH)
};


static inline HandVal 
AStudDeck_AStudRules_EVAL_N( AStudDeck_CardMask cards, int n_cards )
{
  StdDeck_CardMask stdcards;
  HandVal retval;
  uint32 ranks, n_ranks, stdHandType, stdCards;

  /* The strategy here is to first use the standard evaluator, and then
   * make adjustments for the asian stud rules, which are 
   * "flush beats full house" and "A-7-8-9-T straight".  
   * We make use of the assumption that the standard card mask and astud
   * card mask have same layout.  
   */
  stdcards = cards;
  retval = StdDeck_StdRules_EVAL_N(stdcards, n_cards);
  stdHandType = HandVal_HANDTYPE(retval);
  stdCards    = HandVal_CARDS(retval);

  ranks = SC | SD | SH | SS;
  n_ranks = nBitsTable[ranks];

  switch (stdHandType) {
  case StdRules_HandType_QUADS:
  case StdRules_HandType_FLUSH:
    if (n_ranks >= 5) {
      if ((SS & AStudRules_TEN_STRAIGHT) == AStudRules_TEN_STRAIGHT) 
        return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH) 
          + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
      else if ((SC & AStudRules_TEN_STRAIGHT) == AStudRules_TEN_STRAIGHT) 
        return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH) 
          + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
      else if ((SD & AStudRules_TEN_STRAIGHT) == AStudRules_TEN_STRAIGHT) 
        return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH) 
          + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
      else if ((SH & AStudRules_TEN_STRAIGHT) == AStudRules_TEN_STRAIGHT) 
        return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH) 
          + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
    };
    return astudHandTypeMap[stdHandType] + stdCards;
    break;

  case StdRules_HandType_STFLUSH:
  case StdRules_HandType_STRAIGHT:
    return astudHandTypeMap[stdHandType] + stdCards;
    break;

  case StdRules_HandType_FULLHOUSE: 
    if (n_ranks >= 5) {
      if (nBitsTable[SS] >= 5) {
        if ((SS & AStudRules_TEN_STRAIGHT) == AStudRules_TEN_STRAIGHT) 
          return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH) 
            + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
        else 
          return HandVal_HANDTYPE_VALUE(AStudRules_HandType_FLUSH) 
            + topFiveCardsTable[SS];
      }
      else if (nBitsTable[SC] >= 5) {
        if ((SC & AStudRules_TEN_STRAIGHT) == AStudRules_TEN_STRAIGHT) 
          return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH) 
            + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
        else 
          return HandVal_HANDTYPE_VALUE(AStudRules_HandType_FLUSH) 
            + topFiveCardsTable[SC];
      }
      else if (nBitsTable[SD] >= 5) {
        if ((SD & AStudRules_TEN_STRAIGHT) == AStudRules_TEN_STRAIGHT) 
          return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH) 
            + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
        else 
          return HandVal_HANDTYPE_VALUE(AStudRules_HandType_FLUSH) 
            + topFiveCardsTable[SD];
      }
      else if (nBitsTable[SH] >= 5) {
        if ((SH & AStudRules_TEN_STRAIGHT) == AStudRules_TEN_STRAIGHT) 
          return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STFLUSH) 
            + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
        else 
          return HandVal_HANDTYPE_VALUE(AStudRules_HandType_FLUSH) 
            + topFiveCardsTable[SH];
      }
    };
    return HandVal_HANDTYPE_VALUE(AStudRules_HandType_FULLHOUSE) + stdCards;
    break;

  case StdRules_HandType_TRIPS:
  case StdRules_HandType_TWOPAIR:
  case StdRules_HandType_ONEPAIR:
  case StdRules_HandType_NOPAIR:
    if ((ranks & AStudRules_TEN_STRAIGHT) ==  AStudRules_TEN_STRAIGHT) 
      return HandVal_HANDTYPE_VALUE(AStudRules_HandType_STRAIGHT) 
        + HandVal_TOP_CARD_VALUE(AStudDeck_Rank_TEN);
    else
      return astudHandTypeMap[stdHandType] + stdCards;
  };

  return 0;
}

#undef SC
#undef SH
#undef SD
#undef SS

#endif

