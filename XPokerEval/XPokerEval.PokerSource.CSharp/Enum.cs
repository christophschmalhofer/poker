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
  public class Enum
  {
    static public EnumResult Sample(Game game, StdDeck.CardMask[] pockets, StdDeck.CardMask board, StdDeck.CardMask dead, int npockets, int nboard, int niter, int orderflag)
    {
      EnumResult result = new EnumResult();
      API.enumSample(game, pockets, board, dead, npockets, nboard, niter, orderflag, ref result);
      return result;
    }
  }
}
