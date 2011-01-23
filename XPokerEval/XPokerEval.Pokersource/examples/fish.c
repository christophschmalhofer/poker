/* 
 * fish.h
 * Copyright 1999 Brian Goetz
 * 
 * An example program for the poker hand evaluation library.
 * It is so named because it performs the same action as a program written 
 * by Roy Hashimoto a long time ago.  
 * 
 * Given a (possibly empty) set of input cards, a (possibly empty)
 * set of dead cards, and the number of cards in a hand, this program
 * computes the distribution of each type of possible hand type after
 * exhaustively enumerating the set of possible hands which include the input
 * cards and exclude the dead cards.  
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
#include <stdlib.h>
#include <string.h>

#include "poker_defs.h"
#include "inlines/eval_type.h"

int gNCards, gNPegged, gNDead;
CardMask gDeadCards, gPeggedCards;

static void
parseArgs(int argc, char **argv) {
  int i, c;

  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-n") == 0) {
	if (++i == argc) goto error;
	gNCards = atoi(argv[i]);
      } 
      else if (strcmp(argv[i], "-d") == 0) {
	if (++i == argc) goto error;
        if (Deck_stringToCard(argv[i], &c) == 0)
          goto error;
        if (!CardMask_CARD_IS_SET(gDeadCards, c)) {
          CardMask_SET(gDeadCards, c);
          ++gNDead;
        };
      } 
      else 
        goto error;
    } else {
      if (Deck_stringToCard(argv[i], &c) == 0)
        goto error;
      if (!CardMask_CARD_IS_SET(gPeggedCards, c)) {
        CardMask_SET(gPeggedCards, c);
        ++gNPegged;
      };
    }
  }

  return;

 error:
  fprintf(stderr, "Usage: fish [ -d dead-card ] [-n n_cards] [ cards ]\n");
  exit(0);
}


uint32 totals[HandType_LAST+1];

static void dump_totals(void) {
  int i;
  
  for (i = HandType_FIRST; i <= HandType_LAST; i++)
    printf("%s:  %d\n", handTypeNamesPadded[i], totals[i]);
}

int 
main(int argc, char **argv) {
  CardMask hand, deadCards, cards;
  int handtype, nHands=0;

  gNCards = 7;
  CardMask_RESET(gDeadCards);
  CardMask_RESET(gPeggedCards);
  parseArgs(argc, argv);
  CardMask_OR(deadCards, gDeadCards, gPeggedCards);

  /* We use the fast and small EVAL_TYPE evaluator, since we only care 
     about the hand type, not the particular cards.  If we cared about the
     individual cards, we'd use EVAL_N.  
   */
  ENUMERATE_N_CARDS_D(cards, (gNCards-gNPegged), deadCards,
                      {
                        CardMask_OR(hand, cards, gPeggedCards);
                        handtype = Hand_EVAL_TYPE(hand, gNCards);
                        ++nHands;
                        ++totals[handtype];
                      });

  printf("%d boards", nHands);
  if (gNPegged > 0) 
    printf(" containing %s ", Deck_maskString(gPeggedCards));
  if (gNDead) 
    printf(" with %s removed ", Deck_maskString(gDeadCards));
  printf("\n");

  dump_totals();
  return 0;
}
