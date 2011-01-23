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
/* $Id: enumtest1.c 1773 2006-10-12 11:34:41Z loic $ */

#include <stdio.h>
#include <stdlib.h>

#include "combinations.h"

int
main(int argc, char **argv)
{
  int nuniv;
  int nelem;
  int ncombo, *elems, cnum, i;
  Combinations vp;

  if (argc != 3) {
    printf("usage: %s nuniv nelem\n", argv[0]);
    return 1;
  }
  nuniv = atoi(argv[1]);
  nelem = atoi(argv[2]);
  vp = init_combinations(nuniv, nelem);
  if (vp == NULL) {
    printf("init_combinations failed\n");
    return 1;
  }
  ncombo = num_combinations(vp);
  elems = (int *) malloc(nelem * sizeof(int));
  for (cnum=0; cnum<ncombo; cnum++) {
    get_combination(vp, cnum, elems);
    for (i=0; i<nelem; i++)
      printf("%d ", elems[i] + 1);
    printf("\n");
  }
  free_combinations(vp);
  return 0;
}
