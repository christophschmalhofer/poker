/*
 *  five_card_hands.c: Enumerate and tabulate five-card hands
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
#include	<string.h>
#include	<signal.h>

#include	"poker_defs.h"
#if defined(_MSC_VER) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#include	"getopt_w32.h"
#else
#include        <getopt.h>
#endif
#include	"inlines/eval.h"

/* #define VERBOSE */

uint32 totals[HandType_LAST+1];
int num_dead;
CardMask dead_cards;

const char *opts = "d:";

/*
 * returns number of dead cards, -1 on error
 *
 * argc, argv are the standard arguments to main
 * num_dead is a return value of the number of dead cards
 * dead_cards is a return value of the dead cards
 */
static int
parse_args(int argc, char **argv, int *num_dead, CardMask *dead_cards) {
  int i, c, o, rc, len;

  if (num_dead == NULL) {
    return (-1);
  }

  if (dead_cards == NULL) {
    return (-1);
  }

  *num_dead = 0;
  CardMask_RESET(*dead_cards);

  /*
   * parse any options passed to us.
   * -d "c1 [...]" is dead cards
   */
  while((o = getopt(argc, argv, opts)) != -1) {
    switch(o) {
    case 'd':
      len = strlen(optarg);
      for(i = 0;i < len;) {
	rc = StdDeck_stringToCard(optarg+i, &c);
	if (rc) {
	  StdDeck_CardMask_SET(*dead_cards, c);
	  (*num_dead)++;
	  i += 2;
	} else {
	  i++;
	}
      }
      break;
    }
  }

  return (*num_dead);
}

void dump_totals(void) {
  int i;
  
  for (i = HandType_FIRST; i <= HandType_LAST; i++)
    printf("%s:  %d\n", handTypeNamesPadded[i], totals[i]);
}


#ifdef VERBOSE
#define DUMP_HAND do {                          \
  Deck_printMask(cards);                        \
  printf(": ");                                 \
  HandVal_print(handval);                       \
  printf("\n");                                 \
} while (0)
#else
#define DUMP_HAND do { } while (0)
#endif

int main(int argc, char *argv[])
{
  CardMask cards, dead_cards;
  HandVal  handval;
  int      num_dead;

  parse_args(argc, argv, &num_dead, &dead_cards);

  ENUMERATE_5_CARDS_D(cards, dead_cards,
                    {
                      handval = Hand_EVAL_N(cards, 5);
                      ++totals[HandVal_HANDTYPE(handval)];
                      DUMP_HAND;
                    });

  dump_totals();
  exit(0);
}
