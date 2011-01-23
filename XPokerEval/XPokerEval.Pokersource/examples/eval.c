/* 
 * eval.c
 * Copyright 1999 Brian Goetz
 * 
 * An example program for the poker hand evaluation library.
 * All it does is evaluate the hand given on the command line
 * and print out the value. 
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

#include "inlines/eval.h"
#include "inlines/eval_low.h"

int gNCards;
CardMask gCards;
int gLow=0;
int gHighLow=0;

static void
parseArgs(int argc, char **argv) {
  int i, c;

  for (i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-low"))
      gLow = 1;
    else if (!strcmp(argv[i], "-hl")) 
      gHighLow = 1;
    else {
      if (Deck_stringToCard(argv[i], &c) == 0)
        goto error;
      if (!CardMask_CARD_IS_SET(gCards, c)) {
        CardMask_SET(gCards, c);
        ++gNCards;
      };
    };
  }
  
  return;

 error:
  fprintf(stderr, "Usage: eval [ -low ] [ -hl ] cards \n");
  exit(0);
}


int 
main(int argc, char **argv) {
  HandVal handval;
  LowHandVal low;

  gNCards = 0;
  CardMask_RESET(gCards);
  parseArgs(argc, argv);

  if (!gLow) {
    handval = Hand_EVAL_N(gCards, gNCards);
    printf("%s: ", Deck_maskString(gCards));
    HandVal_print(handval);                  
    printf("\n");                                 
  };

  if (gLow || gHighLow) {
#if defined(Hand_EVAL_LOW)
    if (gNCards < 5) 
      printf("Not enough cards to evaluate low hand\n");
    else {
      low = Hand_EVAL_LOW(gCards, gNCards);
      printf("%s (low): ", Deck_maskString(gCards));
      LowHandVal_print(low);                  
      printf("\n");                                 
    };
#else
    printf("Low evaluator not available \n");
#endif
  };

  return 0;
}
