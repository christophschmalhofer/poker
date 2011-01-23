/*
 *  Copyright 2006 Loic Dachary <loic@dachary.org> 
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <assert.h>

#include <poker_defs.h>
#include <poker_wrapper.h>

int main() {
  {
    assert(wrap_StdDeck_N_CARDS() == 52);
  }

  {
    int i;
    StdDeck_CardMask c;
    for(i = 0; i < StdDeck_N_CARDS; i++) {
      int suit = StdDeck_SUIT(i);
      int rank = StdDeck_RANK(i);

      StdDeck_CardMask_RESET(c);
      if (suit == StdDeck_Suit_HEARTS)
        c.cards.hearts = (1 << rank);
      else if (suit == StdDeck_Suit_DIAMONDS)
        c.cards.diamonds = (1 << rank);
      else if (suit == StdDeck_Suit_CLUBS)
        c.cards.clubs = (1 << rank);
      else if (suit == StdDeck_Suit_SPADES)
        c.cards.spades = (1 << rank);

      assert(StdDeck_CardMask_EQUAL(wrap_StdDeck_MASK(i), c));    
    }
  }

  {
    assert(wrap_StdDeck_Rank_2() == StdDeck_Rank_2);
    assert(wrap_StdDeck_Rank_3() == StdDeck_Rank_3);
    assert(wrap_StdDeck_Rank_4() == StdDeck_Rank_4);
    assert(wrap_StdDeck_Rank_5() == StdDeck_Rank_5);
    assert(wrap_StdDeck_Rank_6() == StdDeck_Rank_6);
    assert(wrap_StdDeck_Rank_7() == StdDeck_Rank_7);
    assert(wrap_StdDeck_Rank_8() == StdDeck_Rank_8);
    assert(wrap_StdDeck_Rank_9() == StdDeck_Rank_9);
    assert(wrap_StdDeck_Rank_TEN() == StdDeck_Rank_TEN);
    assert(wrap_StdDeck_Rank_JACK() == StdDeck_Rank_JACK);
    assert(wrap_StdDeck_Rank_QUEEN() == StdDeck_Rank_QUEEN);
    assert(wrap_StdDeck_Rank_KING() == StdDeck_Rank_KING);
    assert(wrap_StdDeck_Rank_ACE() == StdDeck_Rank_ACE);
    assert(wrap_StdDeck_Rank_COUNT() == StdDeck_Rank_COUNT);
    assert(wrap_StdDeck_Rank_FIRST() == StdDeck_Rank_FIRST);
    assert(wrap_StdDeck_Rank_LAST() == StdDeck_Rank_LAST);
  }

  {
    int i;
    for(i = 0; i < StdDeck_Suit_COUNT; i++)
      assert(wrap_StdDeck_RANK(i * StdDeck_Rank_COUNT) == StdDeck_Rank_2);
  }

  {
    int i;
    for(i = 0; i < StdDeck_Rank_COUNT; i++)
      assert(wrap_StdDeck_SUIT(i + StdDeck_Rank_COUNT * StdDeck_Suit_DIAMONDS) == StdDeck_Suit_DIAMONDS);
  }

  {
    assert(wrap_StdDeck_MAKE_CARD(StdDeck_Rank_2, StdDeck_Suit_HEARTS) == 0);
  }

  {
    assert(wrap_StdDeck_Suit_HEARTS() == StdDeck_Suit_HEARTS);
    assert(wrap_StdDeck_Suit_DIAMONDS() == StdDeck_Suit_DIAMONDS);
    assert(wrap_StdDeck_Suit_CLUBS() == StdDeck_Suit_CLUBS);
    assert(wrap_StdDeck_Suit_SPADES() == StdDeck_Suit_SPADES);
    assert(wrap_StdDeck_Suit_FIRST() == StdDeck_Suit_FIRST);
    assert(wrap_StdDeck_Suit_LAST() == StdDeck_Suit_LAST);
    assert(wrap_StdDeck_Suit_COUNT() == StdDeck_Suit_COUNT);
  }

  {
    StdDeck_CardMask c;

    StdDeck_CardMask_RESET(c);
    c = wrap_StdDeck_CardMask_SET_HEARTS(c, 1 << StdDeck_Rank_2);
    c = wrap_StdDeck_CardMask_SET_DIAMONDS(c, 1 << StdDeck_Rank_2);
    c = wrap_StdDeck_CardMask_SET_CLUBS(c, 1 << StdDeck_Rank_2);
    c = wrap_StdDeck_CardMask_SET_SPADES(c, 1 << StdDeck_Rank_2);
    assert(wrap_StdDeck_CardMask_HEARTS(c) == 1);
    assert(wrap_StdDeck_CardMask_DIAMONDS(c) == 1);
    assert(wrap_StdDeck_CardMask_CLUBS(c) == 1);
    assert(wrap_StdDeck_CardMask_SPADES(c) == 1);
  }

  {
    StdDeck_CardMask c, d;
    int As, Ac;
    assert(StdDeck_stringToCard("As", &As) == 2);
    assert(StdDeck_stringToCard("Ac", &Ac) == 2);
    c = wrap_StdDeck_CardMask_RESET();
    assert(wrap_StdDeck_CardMask_IS_EMPTY(c));
    c = wrap_StdDeck_CardMask_NOT(c);
    assert(wrap_StdDeck_CardMask_ANY_SET(c, StdDeck_MASK(As)));
    c = wrap_StdDeck_CardMask_RESET();
    c = StdDeck_MASK(As);
    assert(wrap_StdDeck_CardMask_CARD_IS_SET(c, As));
    c = wrap_StdDeck_CardMask_OR(c, StdDeck_MASK(Ac));
    assert(wrap_StdDeck_CardMask_CARD_IS_SET(c, Ac));
    assert(wrap_StdDeck_CardMask_CARD_IS_SET(c, As));
    d = wrap_StdDeck_CardMask_RESET();
    d = wrap_StdDeck_CardMask_SET(d, As);
    d = wrap_StdDeck_CardMask_XOR(d, c);
    assert(wrap_StdDeck_CardMask_CARD_IS_SET(d, As) == 0);
    d = wrap_StdDeck_CardMask_RESET();
    d = wrap_StdDeck_CardMask_SET(d, As);
    d = wrap_StdDeck_CardMask_XOR(d, c);
    assert(wrap_StdDeck_CardMask_EQUAL(d, StdDeck_MASK(Ac)));
    d = wrap_StdDeck_CardMask_UNSET(d, Ac);
    assert(wrap_StdDeck_CardMask_IS_EMPTY(d));
    c = wrap_StdDeck_CardMask_AND(StdDeck_MASK(Ac), StdDeck_MASK(Ac));
    assert(wrap_StdDeck_CardMask_EQUAL(c, StdDeck_MASK(Ac)));
  }

  return 0;
}
