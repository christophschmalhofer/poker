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
#include "evx_defs.h"
#include "mktable.h"

/* 
 * Generate tables used by the evalExactlyN functions.  These have the
 * hand values pre-built, with the hand-type already ORed in, so that
 * the return value can be returned quickly, at the cost of having many
 * large lookup tables.
 */

#define TV_COMMENT_STRING \
 "evxTripsValueTable[].  Maps 13-bit rank masks to a value identifying\n" \
 "the top two bits (kickers) of the input mask and the hand value for TRIPS."
#define TV_FILENAME "t_evx_tripsval"

#define PV_COMMENT_STRING \
 "evxPairValueTable[].  Maps 13-bit rank masks to a value identifying\n" \
 "the top three bits (kickers) of the input mask and the hand value for PAIR."
#define PV_FILENAME "t_evx_pairval"

#define SV_COMMENT_STRING \
 "evxStrValueTable[].  Maps 13-bit rank masks to a value identifying\n" \
 "the high bit of the straight and the hand value for STRAIGHT."
#define SV_FILENAME "t_evx_strval"

#define FC_COMMENT_STRING \
 "evxFlushCardsTable[].  Maps 13-bit rank masks to a value identifying\n" \
 "whether there are five or more bits set, and if so which bits. "
#define FC_FILENAME "t_evx_flushcards"


static void 
doTripsValueTable(void) {
  int i;

  MakeTable_begin("evxTripsValueTable", 
                  TV_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(TV_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    uint32 val, cards;

    cards = i;
    val = top_bit_func(cards);
    cards ^= val;
    val |= top_bit_func(cards);
    if (val)
      val |= EvxHandVal_TRIPS;
    MakeTable_outputUInt32(val);
  };

  MakeTable_end();
}


static void 
doStraightValueTable(void) {
  int i;

  MakeTable_begin("evxStrValueTable", 
                  SV_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(SV_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    uint32 sv, val;

    sv = straight_func(i);
    val = sv ? 
      (1 << sv) | EvxHandVal_STRAIGHT : 0;
    MakeTable_outputUInt32(val);
  };

  MakeTable_end();
}


static void 
doPairValueTable(void) {
  int i;

  MakeTable_begin("evxPairValueTable", 
                  PV_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(PV_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    uint32 retval, cards, temp;

    cards = i;
    retval = top_bit_func(cards);
    cards ^= retval;
    temp = top_bit_func(cards);
    retval ^= temp;
    cards ^= temp;
    retval |= top_bit_func(cards) 
              | EvxHandVal_ONEPAIR;

    MakeTable_outputUInt32(retval);
  };

  MakeTable_end();
}


static void 
doFlushCardsTable(void) {
  int i;

  MakeTable_begin("evxFlushCardsTable", 
                  FC_FILENAME, 
                  "uint32", 
                  StdDeck_N_RANKMASKS);
  MakeTable_comment(FC_COMMENT_STRING);
  for (i=0; i < StdDeck_N_RANKMASKS; i++) {
    uint32 val;

    val = (n_bits_func(i) >= 5) ? i : 0;
    MakeTable_outputUInt32(val);
  };

  MakeTable_end();
}


int 
main(int argc, char **argv) {
  doPairValueTable();
  doStraightValueTable();
  doTripsValueTable();
  doFlushCardsTable();

  return 0;
}

