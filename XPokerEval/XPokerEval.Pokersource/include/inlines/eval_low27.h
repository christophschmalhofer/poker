/*
 *  Copyright (C) 2005 Loic Dachary <loic@dachary.org>
 *                1993-99 Brian Goetz, Keith Miyake, Clifford T. Matthews
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

#ifndef __EVAL_LOW27_H__
#define __EVAL_LOW27_H__

#include "poker_defs.h"
#include <assert.h>

#define SC suits[StdDeck_Suit_CLUBS]
#define SD suits[StdDeck_Suit_DIAMONDS]
#define SH suits[StdDeck_Suit_HEARTS]
#define SS suits[StdDeck_Suit_SPADES]

static inline HandVal 
StdDeck_Lowball27_EVAL_N( StdDeck_CardMask cards, int n_cards )
{
  HandVal retval;
  uint32 ranks, four_mask, three_mask, two_mask, 
    n_dups, n_ranks;
  uint32 suits[StdDeck_Suit_COUNT];

  SS = StdDeck_CardMask_SPADES(cards);
  SC = StdDeck_CardMask_CLUBS(cards);
  SD = StdDeck_CardMask_DIAMONDS(cards);
  SH = StdDeck_CardMask_HEARTS(cards);

  retval = 0;
  ranks = SC | SD | SH | SS;
  n_ranks = nBitsTable[ranks];
  n_dups = n_cards - n_ranks;

  /* Check for straight, flush, or straight flush, and return if we can
     determine immediately that this is the best possible hand 
  */
  if (n_ranks >= 5) {
    uint32 suit;
    for(suit = 0; suit < StdDeck_Suit_COUNT; suit++) {
      if(nBitsTable[suit] >= 5) {
        if(straightTable[suit] && straightTable[suit] != StdDeck_Rank_5) {
          return HandVal_HANDTYPE_VALUE(StdRules_HandType_STFLUSH)
            + HandVal_TOP_CARD_VALUE(straightTable[suit]);
        } else {
          retval = HandVal_HANDTYPE_VALUE(StdRules_HandType_FLUSH) 
            + topFiveCardsTable[suit];
        }
      }
      if(retval) break;
    } 
    if(retval == 0) {
      int st;

      st = straightTable[ranks];
      if(st && st != StdDeck_Rank_5) 
        retval = HandVal_HANDTYPE_VALUE(StdRules_HandType_STRAIGHT)
          + HandVal_TOP_CARD_VALUE(st);
    };

    /* Another win -- if there can't be a FH/Quads (n_dups < 3), 
       which is true most of the time when there is a made hand, then if we've
       found a five card hand, just return.  This skips the whole process of
       computing two_mask/three_mask/etc.
    */
    if (retval && n_dups < 3)
      return retval;
  };

  /*
   * By the time we're here, either: 
     1) there's no five-card hand possible (flush or straight), or
     2) there's a flush or straight, but we know that there are enough
        duplicates to make a full house / quads possible.  
   */
  switch (n_dups)
    {
    case 0:
      /* It's a no-pair hand */
      return HandVal_HANDTYPE_VALUE(StdRules_HandType_NOPAIR)
        + topFiveCardsTable[ranks];
      break;
      
    case 1: {
      /* It's a one-pair hand */
      uint32 t, kickers;

      two_mask   = ranks ^ (SC ^ SD ^ SH ^ SS);

      retval = HandVal_HANDTYPE_VALUE(StdRules_HandType_ONEPAIR)
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

      two_mask   = ranks ^ (SC ^ SD ^ SH ^ SS);
      if (two_mask) { 
        uint32 t;

        t = ranks ^ two_mask; /* Exactly two bits set in two_mask */
        retval = HandVal_HANDTYPE_VALUE(StdRules_HandType_TWOPAIR)
          + (topFiveCardsTable[two_mask]
             & (HandVal_TOP_CARD_MASK | HandVal_SECOND_CARD_MASK))
          + HandVal_THIRD_CARD_VALUE(topCardTable[t]);

        return retval;
      }
      else {
        int t, second;
        
        three_mask = (( SC&SD )|( SH&SS )) & (( SC&SH )|( SD&SS ));
        
        retval = HandVal_HANDTYPE_VALUE(StdRules_HandType_TRIPS)
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
        retval = HandVal_HANDTYPE_VALUE(StdRules_HandType_QUADS)
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
        retval  = HandVal_HANDTYPE_VALUE(StdRules_HandType_FULLHOUSE);
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
          
        retval = HandVal_HANDTYPE_VALUE(StdRules_HandType_TWOPAIR);
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
  assert(!"Logic error in StdDeck_Lowball27_EVAL_N");
}

#undef SC
#undef SH
#undef SD
#undef SS

#endif

