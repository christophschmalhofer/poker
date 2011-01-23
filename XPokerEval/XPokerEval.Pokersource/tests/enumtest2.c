/*
 *  Copyright 2006 Michael Maurer <mjmaurer@yahoo.com>, 
 *                 Brian Goetz <brian@quiotix.com>, 
 *                 Loic Dachary <loic@dachary.org>, 
 *                 Tim Showalter <tjs@psaux.com>
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
/* $Id: enumtest2.c 1773 2006-10-12 11:34:41Z loic $
   enumtest1.c -- test enumerate macros
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	"poker_defs.h"
#include        "enumerate.h"

#define NPLAYERS 3

int main(void)
{
  int niter;
  StdDeck_CardMask set_var[NPLAYERS];
  int num_sets = NPLAYERS;
  int set_sizes[NPLAYERS];
  StdDeck_CardMask dead_cards;
  StdDeck_CardMask_RESET(dead_cards);

  set_sizes[0] = 1;
  set_sizes[1] = 0;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUMATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 2;
  set_sizes[1] = 0;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 1;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 2;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 3;
  set_sizes[2] = 0;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 1;
  set_sizes[2] = 1;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);

  set_sizes[0] = 1;
  set_sizes[1] = 1;
  set_sizes[2] = 2;
  niter = 0;
  DECK_ENUMERATE_PERMUTATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("PERMUTATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  niter = 0;
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets, set_sizes,
                                dead_cards, {niter++;});
  printf("COMBINATIONS set_sizes=[%d,%d,%d] niter=%d\n",
         set_sizes[0], set_sizes[1], set_sizes[2], niter);
  return 0;
}


