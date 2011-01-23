/*
 * Copyright (C) 2002 Michael Maurer <mjmaurer@yahoo.com>
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
#ifndef __EVAL_JOKER_H__
#define __EVAL_JOKER_H__

#include <assert.h>
#include "deck_joker.h"
#include "rules_joker.h"
#include "inlines/eval.h"

/*
 * JokerDeck evaluator.  
 */


#define SC sc
#define SD sd
#define SH sh
#define SS ss

static inline int 
__topUnset(uint32 ranks) {
  int j;

  for (j=JokerDeck_Rank_ACE; j >= JokerDeck_Rank_2; j--) 
    if (!(ranks & (1 << j))) 
      return j;

  return JokerDeck_Rank_2;
};

static inline HandVal
__flushVal(uint32 ranks) {
  return HandVal_HANDTYPE_VALUE(JokerRules_HandType_FLUSH) 
    + topFiveCardsTable[ranks | (1 << __topUnset(ranks))];
}


static inline HandVal 
JokerDeck_JokerRules_EVAL_N(JokerDeck_CardMask cards, int n_cards) {
  uint32 ranks, ss, sh, sd, sc, jrank, 
    n_ranks, n_dups, two_mask, three_mask, four_mask;
  HandVal retval;

  if (!JokerDeck_CardMask_JOKER(cards)) {
    StdDeck_CardMask sCards;
    
    JokerDeck_CardMask_toStd(cards, sCards);
    return StdDeck_StdRules_EVAL_N(sCards, n_cards);
  };

  /* OK, we know we have a joker */

  ss = JokerDeck_CardMask_SPADES(cards);
  sc = JokerDeck_CardMask_CLUBS(cards);
  sd = JokerDeck_CardMask_DIAMONDS(cards);
  sh = JokerDeck_CardMask_HEARTS(cards);

  retval = 0;
  ranks = SC | SD | SH | SS;
  n_ranks = nBitsTable[ranks];

  /* Check for straight, flush, or straight flush */
  if (n_ranks >= 4) {
    if (nBitsTable[SS] >= 4) {
      if (jokerStraightTable[SS]) 
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_STFLUSH)
          + HandVal_TOP_CARD_VALUE(jokerStraightTable[SS]);
      else
        retval = __flushVal(SS);
    } 
    else if (nBitsTable[SC] >= 4) {
      if (jokerStraightTable[SC]) 
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_STFLUSH)
          + HandVal_TOP_CARD_VALUE(jokerStraightTable[SC]);
      else 
        retval = __flushVal(SC);
    } 
    else if (nBitsTable[SD] >= 4) {
      if (jokerStraightTable[SD]) 
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_STFLUSH)
          + HandVal_TOP_CARD_VALUE(jokerStraightTable[SD]);
      else 
        retval = __flushVal(SD);
    } 
    else if (nBitsTable[SH] >= 4) {
      if (jokerStraightTable[SH]) 
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_STFLUSH)
          + HandVal_TOP_CARD_VALUE(jokerStraightTable[SH]);
      else 
        retval = __flushVal(SH);
    } 
    else {
      int st;

      st = jokerStraightTable[ranks];
      if (st) 
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_STRAIGHT)
          + HandVal_TOP_CARD_VALUE(st);
    };
  };

  /* OK, lets add an ace that's not already set and see what happens; if 
     all the aces are already set, then we have quints and can return */
  jrank = 1 << JokerDeck_Rank_ACE;
  if (!(ss & jrank))
    ss |= jrank;
  else if (!(sc & jrank))
    sc |= jrank;
  else if (!(sd & jrank))
    sd |= jrank;
  else if (!(sh & jrank))
    sh |= jrank;
  else 
    return HandVal_HANDTYPE_VALUE(JokerRules_HandType_QUINTS) 
      + HandVal_TOP_CARD_VALUE(JokerDeck_Rank_ACE);

  ranks |= jrank;
  n_ranks = nBitsTable[ranks];
  n_dups = n_cards - n_ranks;

  switch (n_dups)
    {
    case 0:
      /* no pair */
      if (retval)
        return retval;
      else 
        return HandVal_HANDTYPE_VALUE(JokerRules_HandType_NOPAIR)
          + topFiveCardsTable[ranks];
      break;
      
    case 1: {
      /* one pair */
      uint32 t, kickers;

      if (retval)
        return retval;

      two_mask   = ranks ^ (SC ^ SD ^ SH ^ SS);

      retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_ONEPAIR)
        + HandVal_TOP_CARD_VALUE(topCardTable[two_mask]);
      t = ranks ^ two_mask;      /* Only one bit set in two_mask */
      /* Get the top five cards in what is left, drop all but the top three 
       * cards, and shift them by one to get the three desired kickers */
      kickers = (topFiveCardsTable[t] >> HandVal_CARD_WIDTH)
        & ~HandVal_FIFTH_CARD_MASK;
      retval += kickers;

      return retval;
    }
    break;
      
    case 2: 
      /* Either two pair or trips */

      if (retval)
        return retval;

      two_mask   = ranks ^ (SC ^ SD ^ SH ^ SS);
      if (two_mask) { 
        uint32 t;

        t = ranks ^ two_mask; /* Exactly two bits set in two_mask */
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_TWOPAIR)
          + (topFiveCardsTable[two_mask]
             & (HandVal_TOP_CARD_MASK | HandVal_SECOND_CARD_MASK))
          + HandVal_THIRD_CARD_VALUE(topCardTable[t]);

        return retval;
      }
      else {
        int t, second;
        
        three_mask = (( SC&SD )|( SH&SS )) & (( SC&SH )|( SD&SS ));
        
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_TRIPS)
          + HandVal_TOP_CARD_VALUE(topCardTable[three_mask]);

        t = ranks ^ three_mask; /* Only one bit set in three_mask */
        second = topCardTable[t];
        retval += HandVal_SECOND_CARD_VALUE(second);
        t ^= (1 << second);
        retval += HandVal_THIRD_CARD_VALUE(topCardTable[t]);
        return retval;
      }
      break;
      
    default:
      /* Possible quads, fullhouse, straight or flush, or two pair */
      four_mask  = SH & SD & SC & SS;
      if (four_mask) {
        int tc;

        tc = topCardTable[four_mask];
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_QUADS)
          + HandVal_TOP_CARD_VALUE(tc)
          + HandVal_SECOND_CARD_VALUE(topCardTable[ranks ^ (1 << tc)]);
        return retval;
      };

      /* Technically, three_mask as defined below is really the set of
         bits which are set in three or four of the suits, but since
         we've already eliminated quads, this is OK */
      /* Similarly, two_mask is really two_or_four_mask, but since we've
         already eliminated quads, we can use this shortcut */

      two_mask   = ranks ^ (SC ^ SD ^ SH ^ SS);
      if (nBitsTable[two_mask] != n_dups) {
        /* Must be some trips then, which really means there is a 
           full house since n_dups >= 3 */
        int tc, t;

        three_mask = (( SC&SD )|( SH&SS )) & (( SC&SH )|( SD&SS ));
        retval  = HandVal_HANDTYPE_VALUE(JokerRules_HandType_FULLHOUSE);
        tc = topCardTable[three_mask];
        retval += HandVal_TOP_CARD_VALUE(tc);
        t = (two_mask | three_mask) ^ (1 << tc);
        retval += HandVal_SECOND_CARD_VALUE(topCardTable[t]);
        return retval;
      };

      if (retval) /* flush and straight */
        return retval;
      else {
        /* Must be two pair */
        int top, second;
          
        retval = HandVal_HANDTYPE_VALUE(JokerRules_HandType_TWOPAIR);
        top = topCardTable[two_mask];
        retval += HandVal_TOP_CARD_VALUE(top);
        second = topCardTable[two_mask ^ (1 << top)];
        retval += HandVal_SECOND_CARD_VALUE(second);
        retval += HandVal_THIRD_CARD_VALUE(topCardTable[ranks ^ (1 << top) 
                                                        ^ (1 << second)]);
        return retval;
      };

      break;
    };

  /* Should never happen */
  assert(!"Logic error in JokerDeck_JokerRules_EVAL_N");
  return retval;
}

#undef SC
#undef SH
#undef SD
#undef SS

#endif
