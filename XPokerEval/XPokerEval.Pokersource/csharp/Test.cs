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
using NUnit.Framework;

namespace PokerEval
{
  [TestFixture]
  public class Test
  {
    [SetUp]
    public void SetUp()
    {
    }
    [TearDown]
    public void TearDown()
    {
    }
    [Test]
    public void TestEnumResult()
    {
      EnumResult result = new EnumResult();
      result.Clear();
      StdDeck.CardMask[] pockets = {};
      StdDeck.CardMask board = new StdDeck.CardMask();
      result.Print(pockets, board);
    }
    [Test]
    public void TestString2Card2String()
    {
      string cardStr = "As";
      int card = StdDeck.StringToCard(cardStr);
      string cardStr2 = StdDeck.CardToString(card);
      Assert.AreEqual(cardStr, cardStr2);
    }
    [Test]
    public void TestCardMask()
    {
      int card = StdDeck.StringToCard("As");
      StdDeck.CardMask mask = new StdDeck.CardMask();
      mask = API.wrap_StdDeck_CardMask_SET(mask, card);
      card = StdDeck.StringToCard("Ac");
      mask = API.wrap_StdDeck_CardMask_SET(mask, card);
      mask = API.wrap_StdDeck_CardMask_RESET();
      Assert.AreEqual(0, mask.mask);
    }
    [Test]
    public void TestPokenum()
    {
      StdDeck.CardMask pocket1 = new StdDeck.CardMask();
      pocket1.Set("As");
      pocket1.Set("Ac");
      StdDeck.CardMask pocket2 = new StdDeck.CardMask();
      pocket2.Set("Qs");
      pocket2.Set("Qc");
      StdDeck.CardMask[] pockets = {pocket1, pocket2};
      StdDeck.CardMask board = new StdDeck.CardMask();
      board.Set("Ks");
      board.Set("Kc");
      board.Set("Kh");
      StdDeck.CardMask dead = new StdDeck.CardMask();
      dead.Set("As");
      dead.Set("Ac");
      dead.Set("Qs");
      dead.Set("Qc");
      dead.Set("Ks");
      dead.Set("Kc");
      dead.Set("Kh");
      int npockets = 2;
      int nboard = 3;
      int niter = 1;
      int orderflag = 0;
      EnumResult result = Enum.Sample(Game.holdem, pockets, board, dead, npockets, nboard, niter, orderflag);
      result.Print(pockets, board);
    }
    [Test]
    public void Test_wrap_StdDeck_N_CARDS()
    {
      int ncards = API.wrap_StdDeck_N_CARDS();
      Assert.AreEqual(52, ncards);
    }
    [Test]
    public void Test_wrap_StdDeck_Mask()
    {
      long[] cardMasksTable = { 
      0x0001000000000000LL  ,
      0x0002000000000000LL  ,
      0x0004000000000000LL  ,
      0x0008000000000000LL  ,
      0x0010000000000000LL  ,
      0x0020000000000000LL  ,
      0x0040000000000000LL  ,
      0x0080000000000000LL  ,
      0x0100000000000000LL  ,
      0x0200000000000000LL  ,
      0x0400000000000000LL  ,
      0x0800000000000000LL  ,
      0x1000000000000000LL  ,
      0x0000000100000000LL  ,
      0x0000000200000000LL  ,
      0x0000000400000000LL  ,
      0x0000000800000000LL  ,
      0x0000001000000000LL  ,
      0x0000002000000000LL  ,
      0x0000004000000000LL  ,
      0x0000008000000000LL  ,
      0x0000010000000000LL  ,
      0x0000020000000000LL  ,
      0x0000040000000000LL  ,
      0x0000080000000000LL  ,
      0x0000100000000000LL  ,
      0x0000000000010000LL  ,
      0x0000000000020000LL  ,
      0x0000000000040000LL  ,
      0x0000000000080000LL  ,
      0x0000000000100000LL  ,
      0x0000000000200000LL  ,
      0x0000000000400000LL  ,
      0x0000000000800000LL  ,
      0x0000000001000000LL  ,
      0x0000000002000000LL  ,
      0x0000000004000000LL  ,
      0x0000000008000000LL  ,
      0x0000000010000000LL  ,
      0x0000000000000001LL  ,
      0x0000000000000002LL  ,
      0x0000000000000004LL  ,
      0x0000000000000008LL  ,
      0x0000000000000010LL  ,
      0x0000000000000020LL  ,
      0x0000000000000040LL  ,
      0x0000000000000080LL  ,
      0x0000000000000100LL  ,
      0x0000000000000200LL  ,
      0x0000000000000400LL  ,
      0x0000000000000800LL  ,
      0x0000000000001000LL  
      };
      for (int i = 0; i < cardMasksTable.Length; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  Assert.AreEqual(cardMasksTable[i], card.mask);
	}
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_2()
    {
      int rank = API.wrap_StdDeck_Rank_2();
      Assert.AreEqual(0, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_3()
    {
      int rank = API.wrap_StdDeck_Rank_3();
      Assert.AreEqual(1, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_4()
    {
      int rank = API.wrap_StdDeck_Rank_4();
      Assert.AreEqual(2, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_5()
    {
      int rank = API.wrap_StdDeck_Rank_5();
      Assert.AreEqual(3, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_6()
    {
      int rank = API.wrap_StdDeck_Rank_6();
      Assert.AreEqual(4, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_7()
    {
      int rank = API.wrap_StdDeck_Rank_7();
      Assert.AreEqual(5, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_8()
    {
      int rank = API.wrap_StdDeck_Rank_8();
      Assert.AreEqual(6, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_9()
    {
      int rank = API.wrap_StdDeck_Rank_9();
      Assert.AreEqual(7, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_TEN()
    {
      int rank = API.wrap_StdDeck_Rank_TEN();
      Assert.AreEqual(8, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_JACK()
    {
      int rank = API.wrap_StdDeck_Rank_JACK();
      Assert.AreEqual(9, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_QUEEN()
    {
      int rank = API.wrap_StdDeck_Rank_QUEEN();
      Assert.AreEqual(10, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_KING()
    {
      int rank = API.wrap_StdDeck_Rank_KING();
      Assert.AreEqual(11, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_ACE()
    {
      int rank = API.wrap_StdDeck_Rank_ACE();
      Assert.AreEqual(12, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_COUNT()
    {
      int rank = API.wrap_StdDeck_Rank_COUNT();
      Assert.AreEqual(13, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_FIRST()
    {
      int rank = API.wrap_StdDeck_Rank_FIRST();
      Assert.AreEqual(0, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_Rank_LAST()
    {
      int rank = API.wrap_StdDeck_Rank_LAST();
      Assert.AreEqual(12, rank);
    }
    [Test]
    public void Test_wrap_StdDeck_RANK()
    {
      for (int i = 0; i < 52; ++i)
	{
	  int rank = API.wrap_StdDeck_RANK(i);
	  Assert.AreEqual(i % 13, rank);
	}
    }
    [Test]
    public void Test_wrap_StdDeck_SUIT()
    {
      for (int i = 0; i < 52; ++i)
	{
	  int rank = API.wrap_StdDeck_SUIT(i);
	  Assert.AreEqual(i / 13, rank);
	}
    }
    [Test]
    public void Test_wrap_StdDeck_MAKE_CARD()
    {
      for (int rank = 0; rank < 13; ++rank)
	{
	  for (int suit = 0; suit < 4; ++suit)
	    {
	      int card = API.wrap_StdDeck_MAKE_CARD(rank, suit);
	      Assert.AreEqual(13*suit+rank, card);
	    }
	}
    }
    [Test]
    public void Test_wrap_StdDeck_Suit_HEARTS()
    {
      int suit = API.wrap_StdDeck_Suit_HEARTS();
      Assert.AreEqual(0, suit);
    }
    [Test]
    public void Test_wrap_StdDeck_Suit_DIAMONDS()
    {
      int suit = API.wrap_StdDeck_Suit_DIAMONDS();
      Assert.AreEqual(1, suit);
    }
    [Test]
    public void Test_wrap_StdDeck_Suit_CLUBS()
    {
      int suit = API.wrap_StdDeck_Suit_CLUBS();
      Assert.AreEqual(2, suit);
    }
    [Test]
    public void Test_wrap_StdDeck_Suit_SPADES()
    {
      int suit = API.wrap_StdDeck_Suit_SPADES();
      Assert.AreEqual(3, suit);
    }
    [Test]
    public void Test_wrap_StdDeck_Suit_FIRST()
    {
      int suit = API.wrap_StdDeck_Suit_FIRST();
      Assert.AreEqual(0, suit);
    }
    [Test]
    public void Test_wrap_StdDeck_Suit_LAST()
    {
      int suit = API.wrap_StdDeck_Suit_LAST();
      Assert.AreEqual(3, suit);
    }
    [Test]
    public void Test_wrap_StdDeck_Suit_COUNT()
    {
      int suit = API.wrap_StdDeck_Suit_COUNT();
      Assert.AreEqual(4, suit);
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_SPADES()
    {
     for (int i = 0; i < 39; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_SPADES(card);
	  Assert.AreEqual(0, bit);
	}
     for (int i = 39; i < 52; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_SPADES(card);
	  Assert.AreEqual(1<<(i-39), bit);
	}
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_CLUBS()
    {
     for (int i = 0; i < 26; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_CLUBS(card);
	  Assert.AreEqual(0, bit);
	}
     for (int i = 26; i < 39; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_CLUBS(card);
	  Assert.AreEqual(1<<(i-26), bit);
	}
     for (int i = 39; i < 52; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_CLUBS(card);
	  Assert.AreEqual(0, bit);
	}
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_DIAMONDS()
    {
     for (int i = 0; i < 13; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_DIAMONDS(card);
	  Assert.AreEqual(0, bit);
	}
     for (int i = 13; i < 26; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_DIAMONDS(card);
	  Assert.AreEqual(1<<(i-13), bit);
	}
     for (int i = 39; i < 52; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_DIAMONDS(card);
	  Assert.AreEqual(0, bit);
	}
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_HEARTS()
    {
     for (int i = 0; i < 13; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_HEARTS(card);
	  Assert.AreEqual(1<<i, bit);
	}
     for (int i = 13; i < 52; ++i)
	{
	  StdDeck.CardMask card = API.wrap_StdDeck_MASK(i);
	  int bit = API.wrap_StdDeck_CardMask_HEARTS(card);
	  Assert.AreEqual(0, bit);
	}
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_SET_HEARTS()
    {
      StdDeck.CardMask cm = API.wrap_StdDeck_CardMask_RESET();
      for (int i = 0; i < (1<<13)-1; ++i)
	{
	  cm = API.wrap_StdDeck_CardMask_SET_HEARTS(cm, i);
	  int bit = API.wrap_StdDeck_CardMask_HEARTS(cm);
	  Assert.AreEqual(i, bit);	  
	}
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_SET_DIAMONDS()
    {
      StdDeck.CardMask cm = API.wrap_StdDeck_CardMask_RESET();
      for (int i = 0; i < (1<<13)-1; ++i)
	{
	  cm = API.wrap_StdDeck_CardMask_SET_DIAMONDS(cm, i);
	  int bit = API.wrap_StdDeck_CardMask_DIAMONDS(cm);
	  Assert.AreEqual(i, bit);	  
	}
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_SET_CLUBS()
    {
      StdDeck.CardMask cm = API.wrap_StdDeck_CardMask_RESET();
      for (int i = 0; i < (1<<13)-1; ++i)
	{
	  cm = API.wrap_StdDeck_CardMask_SET_CLUBS(cm, i);
	  int bit = API.wrap_StdDeck_CardMask_CLUBS(cm);
	  Assert.AreEqual(i, bit);	  
	}
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_SET_SPADES()
    {
      StdDeck.CardMask cm = API.wrap_StdDeck_CardMask_RESET();
      for (int i = 0; i < (1<<13)-1; ++i)
	{
	  cm = API.wrap_StdDeck_CardMask_SET_SPADES(cm, i);
	  int bit = API.wrap_StdDeck_CardMask_SPADES(cm);
	  Assert.AreEqual(i, bit);	  
	}
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_NOT()
    {
      StdDeck.CardMask spades = API.wrap_StdDeck_CardMask_RESET();
      spades = API.wrap_StdDeck_CardMask_SET_SPADES(spades, 11);
      StdDeck.CardMask cm = API.wrap_StdDeck_CardMask_NOT(spades);
      Assert.AreEqual(~11&((1<<13)-1), API.wrap_StdDeck_CardMask_SPADES(cm));
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_OR()
    {
      StdDeck.CardMask hearts = API.wrap_StdDeck_CardMask_RESET();
      StdDeck.CardMask spades = API.wrap_StdDeck_CardMask_RESET();
      spades = API.wrap_StdDeck_CardMask_SET_SPADES(spades, 11);
      hearts = API.wrap_StdDeck_CardMask_SET_HEARTS(hearts, 12);
      StdDeck.CardMask cm = API.wrap_StdDeck_CardMask_OR(spades, hearts);
      Assert.AreEqual(11, API.wrap_StdDeck_CardMask_SPADES(cm));
      Assert.AreEqual(12, API.wrap_StdDeck_CardMask_HEARTS(cm));
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_AND()
    {
      StdDeck.CardMask spades1 = API.wrap_StdDeck_CardMask_RESET();
      StdDeck.CardMask spades2 = API.wrap_StdDeck_CardMask_RESET();
      spades1 = API.wrap_StdDeck_CardMask_SET_SPADES(spades1, 7);
      spades2 = API.wrap_StdDeck_CardMask_SET_SPADES(spades2, 9);
      StdDeck.CardMask cm = API.wrap_StdDeck_CardMask_AND(spades1, spades2);
      Assert.AreEqual(1, API.wrap_StdDeck_CardMask_SPADES(cm));
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_XOR()
    {
      StdDeck.CardMask spades1 = API.wrap_StdDeck_CardMask_RESET();
      StdDeck.CardMask spades2 = API.wrap_StdDeck_CardMask_RESET();
      spades1 = API.wrap_StdDeck_CardMask_SET_SPADES(spades1, 7);
      spades2 = API.wrap_StdDeck_CardMask_SET_SPADES(spades2, 1);
      StdDeck.CardMask cm = API.wrap_StdDeck_CardMask_XOR(spades1, spades2);
      Assert.AreEqual(6, API.wrap_StdDeck_CardMask_SPADES(cm));
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_SET()
    {
      StdDeck.CardMask mask = API.wrap_StdDeck_CardMask_RESET();
      mask = API.wrap_StdDeck_CardMask_SET(mask, 10);
      Assert.AreEqual(API.wrap_StdDeck_MASK(10), mask);
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_UNSET()
    {
      StdDeck.CardMask mask = API.wrap_StdDeck_CardMask_RESET();
      mask = API.wrap_StdDeck_CardMask_SET(mask, 10);
      Assert.AreEqual(API.wrap_StdDeck_MASK(10), mask);
      mask = API.wrap_StdDeck_CardMask_UNSET(mask, 10);
      Assert.AreEqual(API.wrap_StdDeck_CardMask_RESET(), mask);
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_CARD_IS_SET()
    {
      StdDeck.CardMask mask = API.wrap_StdDeck_CardMask_RESET();
      mask = API.wrap_StdDeck_CardMask_SET(mask, 10);
      Assert.AreEqual(0, API.wrap_StdDeck_CardMask_CARD_IS_SET(mask, 11));
      Assert.AreEqual(1, API.wrap_StdDeck_CardMask_CARD_IS_SET(mask, 10));
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_ANY_SET()
    {
      StdDeck.CardMask mask = API.wrap_StdDeck_CardMask_RESET();
      mask = API.wrap_StdDeck_CardMask_SET(mask, 10);
      mask = API.wrap_StdDeck_CardMask_SET(mask, 11);
      mask = API.wrap_StdDeck_CardMask_SET(mask, 12);
      StdDeck.CardMask ok = API.wrap_StdDeck_CardMask_RESET();
      ok = API.wrap_StdDeck_CardMask_SET(ok, 11);
      ok = API.wrap_StdDeck_CardMask_SET(ok, 12);
      ok = API.wrap_StdDeck_CardMask_SET(ok, 13);
      Assert.AreEqual(1, API.wrap_StdDeck_CardMask_ANY_SET(mask, ok));
      StdDeck.CardMask ko = API.wrap_StdDeck_CardMask_RESET();
      ko = API.wrap_StdDeck_CardMask_SET(ko, 13);
      Assert.AreEqual(0, API.wrap_StdDeck_CardMask_ANY_SET(mask, ko));
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_RESET()
    {
      StdDeck.CardMask mask = API.wrap_StdDeck_CardMask_RESET();
      Assert.AreEqual(0, mask.mask);
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_IS_EMPTY()
    {
      StdDeck.CardMask mask = API.wrap_StdDeck_CardMask_RESET();
      Assert.AreEqual(1, API.wrap_StdDeck_CardMask_IS_EMPTY(mask));
      mask = API.wrap_StdDeck_CardMask_SET(mask, 12);
      Assert.AreEqual(0, API.wrap_StdDeck_CardMask_IS_EMPTY(mask));
    }
    [Test]
    public void Test_wrap_StdDeck_CardMask_EQUAL()
    {
      StdDeck.CardMask one = API.wrap_StdDeck_CardMask_RESET();
      StdDeck.CardMask two = API.wrap_StdDeck_CardMask_RESET();
      one = API.wrap_StdDeck_CardMask_SET(one, 12);
      one = API.wrap_StdDeck_CardMask_SET(one, 13);
      two = API.wrap_StdDeck_CardMask_SET(two, 12);
      Assert.AreEqual(0, API.wrap_StdDeck_CardMask_EQUAL(one, two));
      two = API.wrap_StdDeck_CardMask_SET(two, 13);
      Assert.AreEqual(1, API.wrap_StdDeck_CardMask_EQUAL(one, two));
    }
  }
}
