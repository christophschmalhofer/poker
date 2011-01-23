//
// Copyright (C) 2004-2006 Mekensleep
//
// Mekensleep
// 24 rue vieille du temple
// 75004 Paris
//       licensing@mekensleep.com
//
//  This package is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; version 2 dated June, 1991.
//
//  This package is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this package; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
//  MA 02110-1301, USA.
//
// Authors:
//  Johan Euphrosine <johan@mekensleep.com>
//
// 

using System;

namespace PokerEval
{
  public struct EnumResult
  {
    int game;
    int sampleType;
    int nsamples;
    int nplayers;
    fixed int nwinhi[12];	/* qualifies for high and wins (no tie) */
    fixed int ntiehi[12];	/* qualifies for high and ties */
    fixed int nlosehi[12];	/* qualifies for high and loses */
    fixed int nwinlo[12];	/* qualifies for low and wins (no tie) */
    fixed int ntielo[12];	/* qualifies for low and ties */
    fixed int nloselo[12];	/* qualifies for low and loses */
    fixed int nscoop[12];	/* wins entire pot */
    
    /* nsharehi[i][H] is the number of times that player i tied for the best
       high hand with H total players (including player i), or received no
       share of the pot if H=0; likewise for nsharelo. */
    fixed int nsharehi[12*13];
    fixed int nsharelo[12*13];
    
    /* nshare[i][H][L] is the number of times that player i tied for the best
       high hand with H total players (including player i) and simultaneously
       tied for the best low hand with L total players (including player i),
       where H=0 and L=0 indicate that player i did not win the corresponding
       share of the pot.  For example, nshare[i][1][1] is the number of times
       that player i scooped both high and low; nshare[i][1][2] is the number of
       times that player i won high and split low with one player.  Note that
       the H=0 and L=0 buckets include cases where player i didn't qualify
       (e.g., for an 8-or-better low), which differs from the definition of
       nlosehi[] and nloselo[] above.  So you can't compute ev[] from
       nshare[][][]. */
    fixed int nshare[12*13*13];
    
    /* ev[i] is the pot equity of player i averaged over all outcomes */
    fixed double ev[12];
    public void Print(StdDeck.CardMask[] pockets, StdDeck.CardMask board)
    {
      API.enumResultPrint(ref this, pockets, board);
    }
    public void Clear()
    {
      API.enumResultClear(ref this);
    }
  }
}