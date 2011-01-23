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
#ifndef __EVAL_LOW8_H__
#define __EVAL_LOW8_H__

#include "handval_low.h"

static inline LowHandVal 
StdDeck_Lowball8_EVAL(StdDeck_CardMask cards, int n_cards) {
  uint32 ranks, retval;

  ranks = (StdDeck_CardMask_HEARTS(cards) |
           StdDeck_CardMask_DIAMONDS(cards) |
           StdDeck_CardMask_CLUBS(cards) |
           StdDeck_CardMask_SPADES(cards));
  ranks = Lowball_ROTATE_RANKS(ranks);
  retval = bottomFiveCardsTable[ranks];
  if (retval > 0 && retval <= LowHandVal_WORST_EIGHT)
    return LowHandVal_HANDTYPE_VALUE(StdRules_HandType_NOPAIR) + retval;
  else 
    return LowHandVal_NOTHING;
}

#endif
