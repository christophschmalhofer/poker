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
/* $Id: enumtest3.c 1773 2006-10-12 11:34:41Z loic $ */

#include <stdio.h>
#include <stdlib.h>
#include "poker_defs.h"
#include "enumerate.h"

int main() {
  StdDeck_CardMask set_var[2];
  int num_sets = 2;
  int set_sizes[2] = {2, 1};
  StdDeck_CardMask dead_cards;
  int i;
  
  StdDeck_CardMask_RESET(dead_cards);
  for (i=0; i<39; i++) /* remove three suits to reduce output */
    StdDeck_CardMask_SET(dead_cards, i);
  DECK_ENUMERATE_COMBINATIONS_D(StdDeck, set_var, num_sets,
                                set_sizes, dead_cards,
  { 
    for (i=0; i<num_sets; i++) { 
      printf("%s | ", DmaskString(StdDeck, set_var[i])); 
    } 
    printf("\n"); 
  } 
    );
  return 0;
}
