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
using System.Text;

namespace PokerEval
{
    public class StdDeck
    {
        public static string CardToString(int card)
        {
            const int maxCardStringLength = 256;
            StringBuilder sb = new StringBuilder(maxCardStringLength);
            API.StdDeck_cardToString(card, sb);
            return sb.ToString();
        }
        public static int StringToCard(string cardStr)
        {
            int card;
            API.StdDeck_stringToCard(cardStr, out card);
            return card;
        }
        public struct CardMask
        {
            public Int64 mask;
            public void Set(int card)
            {
                mask = API.wrap_StdDeck_CardMask_SET(this, card).mask;
            }
            public void Set(string card)
            {
                Set(StringToCard(card));
            }
            public void Reset()
            {
                mask = API.wrap_StdDeck_CardMask_RESET().mask;
            }
        }
    }
}