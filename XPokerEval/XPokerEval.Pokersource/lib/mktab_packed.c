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
#include <stdio.h>
#include <assert.h>

#include "poker_defs.h"
#include "mktable.h"

#define NBS_COMMENT_STRING \
 "nBitsAndStrTable[].  Maps 13-bit rank masks onto an 8 bit quantity,\n"     \
 "packed with both the number of bits set in the mask (nBitsTable) and\n"    \
 "a value indicating if a straight is present (straightTable). \n"           \
 "It is possible, using just this table and bit masking operations, \n"      \
 "to determine the type of hand.  Since this table is only 8K it should\n"   \
 "in L1 cache on some processors.\n "                                        \
 "The layout of the result is as follows: \n"\
 "  Bit 0: (nBits >= 5)\n"\
 "  Bit 1: is-straight\n"\
 "  Bits 2-6: nBits\n"
#define NBS_FILENAME "t_nbitsandstr"


static void 
doNBitsAndStrTable(void) {
  int i;
  uint8 val, t;

  MakeTable_begin("nBitsAndStrTable", 
                  NBS_FILENAME, 
                  "uint8", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(NBS_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    val = n_bits_func(i);
    assert((val & 0xF0) == 0);
    t = straight_func(i);

    MakeTable_outputUInt8((val << 2) 
                          | (((t != 0) & 0x01) << 1) 
                          | ((val >= 5) & 0x01) );
  };

  MakeTable_end();
}

int 
main(int argc, char **argv) {
  doNBitsAndStrTable();
  return 0;
}

