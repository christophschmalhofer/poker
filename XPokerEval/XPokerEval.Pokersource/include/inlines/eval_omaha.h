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
/* eval_omaha.h -- Omaha High and Omaha High/Low 8-or-better hand evaluators
 *
 * Uses brute-force enumeration of legal hole-card board-card combinations,
 * and is probably at least 60 times slower than Holdem evaluation.  A legal
 * hole-card board-card combination uses exactly 2 hole cards and 3 board
 * cards.
 *
 * Michael Maurer, Mar 2002.
 */

#ifndef __EVAL_OMAHA_H__
#define __EVAL_OMAHA_H__

#include "inlines/eval.h"
#include "inlines/eval_low8.h"
#include "deck_std.h"
#include "rules_std.h"

#define OMAHA_MINHOLE 4		/* must be at least 2 */
#define OMAHA_MAXHOLE 4		/* could be larger */
#define OMAHA_MINBOARD 3	/* must be at least 3 */
#define OMAHA_MAXBOARD 5	/* could be larger */

/* Evaluate an omaha hand for both high and low.  Return nonzero on error.
   If hival is NULL, skips high evaluation; if loval is NULL, skips
   low evaluation.  Low eval could be sped up with 256x256 rank table. */

static inline int
StdDeck_OmahaHiLow8_EVAL(StdDeck_CardMask hole, StdDeck_CardMask board,
                         HandVal *hival, LowHandVal *loval) {
  StdDeck_CardMask allcards;
  LowHandVal allval;
  HandVal curhi, besthi;
  LowHandVal curlo, bestlo;
  StdDeck_CardMask hole1[OMAHA_MAXHOLE];
  StdDeck_CardMask board1[OMAHA_MAXBOARD];
  StdDeck_CardMask n1, n2, n3, n4, n5;
  int nhole, nboard;
  int eligible = 0;
  int i, h1, h2, b1, b2, b3;

  /* pluck out individual cards from hole and board masks, save in arrays */
  nhole = nboard = 0;
  for (i=0; i<StdDeck_N_CARDS; i++) {
    if (StdDeck_CardMask_CARD_IS_SET(hole, i)) {
      if (nhole >= OMAHA_MAXHOLE)
        return 1;                               /* too many hole cards */
      StdDeck_CardMask_RESET(hole1[nhole]);
      StdDeck_CardMask_SET(hole1[nhole], i);
      nhole++;
    }
    if (StdDeck_CardMask_CARD_IS_SET(board, i)) {
      if (StdDeck_CardMask_CARD_IS_SET(hole, i)) /* same card in hole and board */
        return 2;
      if (nboard >= OMAHA_MAXBOARD)
        return 3;                               /* too many board cards */
      StdDeck_CardMask_RESET(board1[nboard]);
      StdDeck_CardMask_SET(board1[nboard], i);
      nboard++;
    }
  }

  if (nhole < OMAHA_MINHOLE || nhole > OMAHA_MAXHOLE)
    return 4;                                   /* wrong # of hole cards */
  if (nboard < OMAHA_MINBOARD || nboard > OMAHA_MAXBOARD)
    return 5;                                   /* wrong # of board cards */

  /* quick test in case no low is possible with all 9 cards */
  if (loval != NULL) {
    StdDeck_CardMask_OR(allcards, hole, board);
    allval = StdDeck_Lowball8_EVAL(allcards, nhole + nboard);
    eligible = (allval != LowHandVal_NOTHING);
  }

  /* loop over all combinations of hole with board (60 for 4 hole cards
     and 5 board cards). */
  besthi = HandVal_NOTHING;
  bestlo = LowHandVal_NOTHING;
  /* {h1,h2} loop over all hole card combinations */
  for (h1=0; h1<nhole-1; h1++) {
    StdDeck_CardMask_RESET(n1);
    StdDeck_CardMask_OR(n1, n1, hole1[h1]);
    for (h2=h1+1; h2<nhole; h2++) {
      StdDeck_CardMask_OR(n2, n1, hole1[h2]);
      /* {b1,b2,b3} loop over all board card combinations */
      for (b1=0; b1<nboard-2; b1++) {
        StdDeck_CardMask_OR(n3, n2, board1[b1]);
        for (b2=b1+1; b2<nboard-1; b2++) {
          StdDeck_CardMask_OR(n4, n3, board1[b2]);
          for (b3=b2+1; b3<nboard; b3++) {
            if (hival != NULL) {
              StdDeck_CardMask_OR(n5, n4, board1[b3]);
              curhi = StdDeck_StdRules_EVAL_N(n5, 5);
              if (curhi > besthi || besthi == HandVal_NOTHING)
                besthi = curhi;
            }
            if (loval != NULL && eligible) {
              curlo = StdDeck_Lowball8_EVAL(n5, 5);
              if (curlo < bestlo || bestlo == LowHandVal_NOTHING)
                bestlo = curlo;
            }
          }
        }
      }
    }
  }
  if (hival != NULL) *hival = besthi;
  if (loval != NULL) *loval = bestlo;
  return 0;
}

/* Evaluate an omaha hand for high only.  Return nonzero on error. */

static inline int
StdDeck_OmahaHi_EVAL(StdDeck_CardMask hole, StdDeck_CardMask board,
                     HandVal *hival) {
  return StdDeck_OmahaHiLow8_EVAL(hole, board, hival, NULL);
}

#endif
