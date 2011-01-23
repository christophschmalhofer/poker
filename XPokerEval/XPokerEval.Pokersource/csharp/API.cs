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
using System.Runtime.InteropServices;
using System.Text;

namespace PokerEval
{
  public enum OrderingMode
  {
    none,
    hi,
    lo,
    hilo
  }
  public enum Game
  {
    /* do not reorder -- some other static arrays depend on this order */
    holdem,
    holdem8,
    omaha,
    omaha8,
    seven_stud,
    seven_stud8,
    seven_studnsq,
    razz,
    five_draw,
    five_draw8,
    five_drawnsq,
    lowball,
    lowball27,
  }
  public class API
  {
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_N_CARDS")]
    public static extern int wrap_StdDeck_N_CARDS();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_MASK")]
    public static extern StdDeck.CardMask wrap_StdDeck_MASK(int index);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_2")]
    public static extern int wrap_StdDeck_Rank_2();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_3")]
    public static extern int wrap_StdDeck_Rank_3();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_4")]
    public static extern int wrap_StdDeck_Rank_4();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_5")]
    public static extern int wrap_StdDeck_Rank_5();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_6")]
    public static extern int wrap_StdDeck_Rank_6();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_7")]
    public static extern int wrap_StdDeck_Rank_7();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_8")]
    public static extern int wrap_StdDeck_Rank_8();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_9")]
    public static extern int wrap_StdDeck_Rank_9();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_TEN")]
    public static extern int wrap_StdDeck_Rank_TEN();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_JACK")]
    public static extern int wrap_StdDeck_Rank_JACK();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_QUEEN")]
    public static extern int wrap_StdDeck_Rank_QUEEN();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_KING")]
    public static extern int wrap_StdDeck_Rank_KING();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_ACE")]
    public static extern int wrap_StdDeck_Rank_ACE();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_COUNT")]
    public static extern int wrap_StdDeck_Rank_COUNT();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_FIRST")]
    public static extern int wrap_StdDeck_Rank_FIRST();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Rank_LAST")]
    public static extern int wrap_StdDeck_Rank_LAST();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_RANK")]
    public static extern int wrap_StdDeck_RANK(int index);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_SUIT")]
    public static extern int wrap_StdDeck_SUIT(int index);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_MAKE_CARD")]
    public static extern int wrap_StdDeck_MAKE_CARD(int rank, int suit);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Suit_HEARTS")]
    public static extern int wrap_StdDeck_Suit_HEARTS();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Suit_DIAMONDS")]
    public static extern int wrap_StdDeck_Suit_DIAMONDS();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Suit_CLUBS")]
    public static extern int wrap_StdDeck_Suit_CLUBS();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Suit_SPADES")]
    public static extern int wrap_StdDeck_Suit_SPADES();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Suit_FIRST")]
    public static extern int wrap_StdDeck_Suit_FIRST();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Suit_LAST")]
    public static extern int wrap_StdDeck_Suit_LAST();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_Suit_COUNT")]
    public static extern int wrap_StdDeck_Suit_COUNT();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_SPADES")]
    public static extern int wrap_StdDeck_CardMask_SPADES(StdDeck.CardMask cm);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_CLUBS")]
    public static extern int wrap_StdDeck_CardMask_CLUBS(StdDeck.CardMask cm);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_DIAMONDS")]
    public static extern int wrap_StdDeck_CardMask_DIAMONDS(StdDeck.CardMask cm);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_HEARTS")]
    public static extern int wrap_StdDeck_CardMask_HEARTS(StdDeck.CardMask cm);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_SET_HEARTS")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_SET_HEARTS(StdDeck.CardMask cm, int ranks);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_SET_DIAMONDS")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_SET_DIAMONDS(StdDeck.CardMask cm, int ranks);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_SET_CLUBS")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_SET_CLUBS(StdDeck.CardMask cm, int ranks);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_SET_SPADES")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_SET_SPADES(StdDeck.CardMask cm, int ranks);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_NOT")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_NOT(StdDeck.CardMask cm);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_OR")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_OR(StdDeck.CardMask op1, StdDeck.CardMask op2);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_AND")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_AND(StdDeck.CardMask op1, StdDeck.CardMask op2);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_XOR")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_XOR(StdDeck.CardMask op1, StdDeck.CardMask op2);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_SET")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_SET(StdDeck.CardMask mask, int index);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_UNSET")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_UNSET(StdDeck.CardMask mask, int index);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_CARD_IS_SET")]
    public static extern int wrap_StdDeck_CardMask_CARD_IS_SET(StdDeck.CardMask mask, int index);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_ANY_SET")]
    public static extern int wrap_StdDeck_CardMask_ANY_SET(StdDeck.CardMask mask1, StdDeck.CardMask mask2);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_RESET")]
    public static extern StdDeck.CardMask wrap_StdDeck_CardMask_RESET();
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_IS_EMPTY")]
    public static extern int wrap_StdDeck_CardMask_IS_EMPTY(StdDeck.CardMask mask);
    [DllImport("poker-eval", EntryPoint="wrap_StdDeck_CardMask_EQUAL")]
    public static extern int wrap_StdDeck_CardMask_EQUAL(StdDeck.CardMask mask1, StdDeck.CardMask mask2);
    [DllImport("poker-eval", EntryPoint="StdDeck_cardToString")]
    public static extern int StdDeck_cardToString(int cardIndex, StringBuilder outString);
    [DllImport("poker-eval", EntryPoint="StdDeck_stringToCard")]
    public static extern int StdDeck_stringToCard(string inString, out int outCard);
    [DllImport("poker-eval", EntryPoint="enumResultPrint")]
    public static extern void enumResultPrint(ref EnumResult result, StdDeck.CardMask[] pockets,
					      StdDeck.CardMask board);
    [DllImport("poker-eval", EntryPoint="enumResultPrintTerse")]
    public static extern void enumResultPrintTerse(ref EnumResult result,
						   StdDeck.CardMask[] pockets,
						   StdDeck.CardMask board);
    [DllImport("poker-eval", EntryPoint="enumResultClear")]
    public static extern void enumResultClear(ref EnumResult result);
    [DllImport("poker-eval", EntryPoint="enumResultFree")]
    public static extern void enumResultFree(ref EnumResult result);
    [DllImport("poker-eval", EntryPoint="enumResultAlloc")]
    public static extern int enumResultAlloc(ref EnumResult result, int nplayers,
					     OrderingMode mode);
    [DllImport("poker-eval", EntryPoint="enumExhaustive")]
    public static extern int enumExhaustive(Game game, StdDeck.CardMask[] pockets,
					    StdDeck.CardMask board, StdDeck.CardMask dead,
					    int npockets, int nboard, int orderflag,
					    ref EnumResult result);
    [DllImport("poker-eval", EntryPoint="enumSample")]
    public static extern int enumSample(Game game, StdDeck.CardMask[] pockets,
					StdDeck.CardMask board, StdDeck.CardMask dead,
					int npockets, int nboard, int niter, int orderflag,
					ref EnumResult result);
    [DllImport("poker-eval", EntryPoint="enumGameParams")]
    public static extern GameParams enumGameParams(Game game);
  }
}
