/*
 *  hcmp2.c: a program to compare two pairs of hold'em hole cards at any
 *           point of the game (pre-flop, on the flop, turn or river).
 *              
 *  Example:
 *
 *      hcmp2  tc ac  3h ah  8c 6h 7h
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

int gNCommon, gNDead;
CardMask gDeadCards, gCommonCards, gPlayerCards[2];


static void
parseArgs(int argc, char **argv) {
  int i, count = 0, c;

  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-d") == 0) {
	if (++i == argc) goto error;
        if (StdDeck_stringToCard(argv[i], &c) == 0)
          goto error;
        if (!CardMask_CARD_IS_SET(gDeadCards, c)) {
          ++gNDead;
          StdDeck_CardMask_SET(gDeadCards, c);
        };
      } 
      else 
        goto error;
    } else {
      if (StdDeck_stringToCard(argv[i], &c) == 0)
        goto error;
      if (count < 2) 
        StdDeck_CardMask_SET(gPlayerCards[0], c);
      else if (count < 4) 
        StdDeck_CardMask_SET(gPlayerCards[1], c);
      else {
        StdDeck_CardMask_SET(gCommonCards, c);
        ++gNCommon;
      };
      ++count;
    }
  }
  if (count < 4) goto error;
  if (gNCommon > 5) goto error;

  return;

 error:
  fprintf(stderr, "Usage: hcmp2 [ -d dead-card ] p1-cards p2-cards [ common-cards ]\n");
  exit(0);
}


int main( int argc, char *argv[] )
{
  CardMask cards, p0, p1, c0, c1;
  HandVal h0, h1;
  int h0_count=0, h1_count=0, tie_count=0, count=0;

  CardMask_RESET(gDeadCards);
  CardMask_RESET(gCommonCards);
  CardMask_RESET(gPlayerCards[0]);
  CardMask_RESET(gPlayerCards[1]);
  parseArgs(argc, argv);

  CardMask_OR(p0, gPlayerCards[0], gCommonCards);
  CardMask_OR(p1, gPlayerCards[1], gCommonCards);
  CardMask_OR(gDeadCards, gDeadCards, gCommonCards);
  CardMask_OR(gDeadCards, gDeadCards, gPlayerCards[0]);
  CardMask_OR(gDeadCards, gDeadCards, gPlayerCards[1]);

  ENUMERATE_N_CARDS_D(cards, 5-gNCommon, gDeadCards, 
                      {
                        ++count;
                        CardMask_OR(c0, p0, cards);
                        h0 = Hand_EVAL_N(c0, 7);
                        CardMask_OR(c1, p1, cards);
                        h1 = Hand_EVAL_N(c1, 7);

                        if (h0 > h1)
			  ++h0_count;
                        else if (h1 > h0)
			  ++h1_count;
                        else
			  ++tie_count;
                      }
                      );

  printf("%d boards", count);
  if (gNCommon > 0) 
    printf(" containing %s ", Deck_maskString(gCommonCards));
  if (gNDead) 
    printf(" with %s removed ", Deck_maskString(gDeadCards));
  printf("\n");

  printf("  cards      win  %%win       loss  %%lose       tie  %%tie      EV\n");
  printf("  %s  %7d %6.2f   %7d %6.2f   %7d %6.2f     %5.3f\n", 
         Deck_maskString(gPlayerCards[0]), 
         h0_count, 100.0*h0_count/count, 
         h1_count, 100.0*h1_count/count, 
         tie_count, 100.0*tie_count/count, 
         (1.0*h0_count + (tie_count/2.0)) / count);

  printf("  %s  %7d %6.2f   %7d %6.2f   %7d %6.2f     %5.3f\n", 
         Deck_maskString(gPlayerCards[1]), 
         h1_count, 100.0*h1_count/count, 
         h0_count, 100.0*h0_count/count, 
         tie_count, 100.0*tie_count/count, 
         (1.0*h1_count + (tie_count/2.0)) / count);

  return 0;
}
