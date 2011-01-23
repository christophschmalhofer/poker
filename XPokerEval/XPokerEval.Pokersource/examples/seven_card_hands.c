/*
 *  seven_card_hands.c: Enumerate and tabulate seven-card hands
 *
 *  Copyright (C) 1993-99 Clifford T. Matthews, Brian Goetz
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


#include	<stdio.h>
#include	<stdlib.h>
#include	<signal.h>
#include	"poker_defs.h"
#include	"inlines/eval.h"

/* #define VERBOSE */

uint32 totals[HandType_LAST+1];


void dump_totals(void) {
  int i;
  
  for (i = HandType_FIRST; i <= HandType_LAST; i++)
    printf("%s:  %d\n", handTypeNamesPadded[i], totals[i]);
}


#ifdef VERBOSE
#define DUMP_HAND do {                          \
  Deck_printMask(cards);                        \
  printf(": ");                                 \
  Rules_printHandval(handval);                  \
  printf("\n");                                 \
} while (0)
#else
#define DUMP_HAND do { } while (0)
#endif

int main( void )
{
  CardMask cards;
  HandVal  handval;

  ENUMERATE_7_CARDS(cards, 
                    {
                      handval = Hand_EVAL_N(cards, 7);
                      ++totals[HandVal_HANDTYPE(handval)];
                      DUMP_HAND;
                    });

  dump_totals();
  exit(0);
}
