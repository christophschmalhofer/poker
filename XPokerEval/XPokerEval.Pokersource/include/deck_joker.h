/*
 * Copyright (C) 2004-2006 
 *           Michael Maurer <mjmaurer@yahoo.com>
 *           Brian Goetz <brian@quiotix.com>
 *           Loic Dachary <loic@dachary.org>
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
#ifndef __DECK_JOKER_H__
#define __DECK_JOKER_H__

#include "pokereval_export.h"

#define JokerDeck_N_CARDS      53
#define JokerDeck_MASK(index)  (JokerDeck_cardMasksTable[index])

#define JokerDeck_Rank_2      StdDeck_Rank_2
#define JokerDeck_Rank_3      StdDeck_Rank_3
#define JokerDeck_Rank_4      StdDeck_Rank_4
#define JokerDeck_Rank_5      StdDeck_Rank_5
#define JokerDeck_Rank_6      StdDeck_Rank_6
#define JokerDeck_Rank_7      StdDeck_Rank_7
#define JokerDeck_Rank_8      StdDeck_Rank_8
#define JokerDeck_Rank_9      StdDeck_Rank_9
#define JokerDeck_Rank_TEN    StdDeck_Rank_TEN
#define JokerDeck_Rank_JACK   StdDeck_Rank_JACK
#define JokerDeck_Rank_QUEEN  StdDeck_Rank_QUEEN
#define JokerDeck_Rank_KING   StdDeck_Rank_KING
#define JokerDeck_Rank_ACE    StdDeck_Rank_ACE
#define JokerDeck_Rank_COUNT  StdDeck_Rank_COUNT
#define JokerDeck_Rank_FIRST  StdDeck_Rank_FIRST
#define JokerDeck_Rank_LAST   StdDeck_Rank_LAST
#define JokerDeck_N_RANKMASKS StdDeck_N_RANKMASKS

#define JokerDeck_RANK          StdDeck_RANK
#define JokerDeck_SUIT          StdDeck_SUIT
#define JokerDeck_IS_JOKER(index) ((index) == JokerDeck_JOKER)
#define JokerDeck_MAKE_CARD     StdDeck_MAKE_CARD
#define JokerDeck_JOKER        (JokerDeck_N_CARDS - 1)

#define JokerDeck_Suit_HEARTS   StdDeck_Suit_HEARTS
#define JokerDeck_Suit_DIAMONDS StdDeck_Suit_DIAMONDS
#define JokerDeck_Suit_CLUBS    StdDeck_Suit_CLUBS
#define JokerDeck_Suit_SPADES   StdDeck_Suit_SPADES
#define JokerDeck_Suit_FIRST    StdDeck_Suit_FIRST
#define JokerDeck_Suit_LAST     StdDeck_Suit_LAST
#define JokerDeck_Suit_COUNT    StdDeck_Suit_COUNT

typedef StdDeck_RankMask JokerDeck_RankMask;

/* 
   It is important that the hearts, spades, clubs, and diamonds fields 
   in JokerDeck_CardMask agree with the definition for StdDeck_CardMask!  
*/
typedef union {
#ifdef USE_INT64
  uint64  cards_n;
#else
  struct {
    uint32 n1, n2;
  } cards_nn;
#endif
  struct {
#ifdef WORDS_BIGENDIAN
    uint32         : 2;
    uint32 joker   : 1;
    uint32 spades  : 13;
    uint32         : 3;
    uint32 clubs   : 13;
    uint32         : 3;
    uint32 diamonds: 13;
    uint32         : 3;
    uint32 hearts  : 13;
#else
    uint32 spades  : 13;
    uint32         : 3;
    uint32 clubs   : 13;
    uint32         : 3;
    uint32 diamonds: 13;
    uint32         : 3;
    uint32 hearts  : 13;
    uint32 joker   : 1;
    uint32         : 2;
#endif
  } cards;
} JokerDeck_CardMask;

#define JokerDeck_CardMask_SPADES(cm)   ((cm).cards.spades)
#define JokerDeck_CardMask_CLUBS(cm)    ((cm).cards.clubs)
#define JokerDeck_CardMask_DIAMONDS(cm) ((cm).cards.diamonds)
#define JokerDeck_CardMask_HEARTS(cm)   ((cm).cards.hearts)
#define JokerDeck_CardMask_JOKER(cm)    ((cm).cards.joker)

#define JokerDeck_CardMask_SET_SPADES(cm, ranks)   ((cm).cards.spades=(ranks))
#define JokerDeck_CardMask_SET_CLUBS(cm, ranks)    ((cm).cards.clubs=(ranks))
#define JokerDeck_CardMask_SET_DIAMONDS(cm, ranks) \
        ((cm).cards.diamonds=(ranks))
#define JokerDeck_CardMask_SET_HEARTS(cm, ranks)   ((cm).cards.hearts=(ranks))
#define JokerDeck_CardMask_SET_JOKER(cm, ranks)    ((cm).cards.joker=(ranks))

#define JokerDeck_CardMask_NOT         StdDeck_CardMask_NOT
#define JokerDeck_CardMask_OR          StdDeck_CardMask_OR
#define JokerDeck_CardMask_AND         StdDeck_CardMask_AND
#define JokerDeck_CardMask_XOR         StdDeck_CardMask_XOR
#define JokerDeck_CardMask_ANY_SET     StdDeck_CardMask_ANY_SET
#define JokerDeck_CardMask_RESET       StdDeck_CardMask_RESET
#define JokerDeck_CardMask_UNSET       StdDeck_CardMask_UNSET
#define JokerDeck_CardMask_IS_EMPTY    StdDeck_CardMask_IS_EMPTY

#define JokerDeck_CardMask_toStd(jCards, sCards) \
do { \
  StdDeck_CardMask_RESET(sCards); \
  StdDeck_CardMask_SET_SPADES(sCards, JokerDeck_CardMask_SPADES(jCards)); \
  StdDeck_CardMask_SET_HEARTS(sCards, JokerDeck_CardMask_HEARTS(jCards)); \
  StdDeck_CardMask_SET_CLUBS(sCards, JokerDeck_CardMask_CLUBS(jCards)); \
  StdDeck_CardMask_SET_DIAMONDS(sCards, JokerDeck_CardMask_DIAMONDS(jCards)); \
} while (0)

#define JokerDeck_CardMask_fromStd(jCards, sCards) \
do { \
  JokerDeck_CardMask_RESET(jCards); \
  JokerDeck_CardMask_SET_SPADES(jCards, StdDeck_CardMask_SPADES(sCards)); \
  JokerDeck_CardMask_SET_HEARTS(jCards, StdDeck_CardMask_HEARTS(sCards)); \
  JokerDeck_CardMask_SET_CLUBS(jCards, StdDeck_CardMask_CLUBS(sCards)); \
  JokerDeck_CardMask_SET_DIAMONDS(jCards, StdDeck_CardMask_DIAMONDS(sCards)); \
} while (0)

#define JokerDeck_CardMask_fromStd_N(jCards, sCards, N) \
do { \
  int _i; \
  for (_i=0; _i<N; _i++) {\
    JokerDeck_CardMask_RESET(jCards[_i]); \
    JokerDeck_CardMask_SET_SPADES(jCards[_i], StdDeck_CardMask_SPADES(sCards[_i])); \
    JokerDeck_CardMask_SET_HEARTS(jCards[_i], StdDeck_CardMask_HEARTS(sCards[_i])); \
    JokerDeck_CardMask_SET_CLUBS(jCards[_i], StdDeck_CardMask_CLUBS(sCards[_i])); \
    JokerDeck_CardMask_SET_DIAMONDS(jCards[_i], StdDeck_CardMask_DIAMONDS(sCards[_i])); \
  }\
} while (0)

#ifdef USE_INT64                                                          
#define JokerDeck_CardMask_CARD_IS_SET(mask, index)                       \
  (( (mask).cards_n & (JokerDeck_MASK(index).cards_n)) != 0 )                 
#else                                                                   
#define JokerDeck_CardMask_CARD_IS_SET(mask, index)                       \
  ((( (mask).cards_nn.n1 & (JokerDeck_MASK(index).cards_nn.n1)) != 0 )    \
   || (( (mask).cards_nn.n2 & (JokerDeck_MASK(index).cards_nn.n2)) != 0 ))   
#endif

#define JokerDeck_CardMask_SET(mask, index)       \
do {                                              \
  JokerDeck_CardMask _t1 = JokerDeck_MASK(index); \
  JokerDeck_CardMask_OR((mask), (mask), _t1);     \
} while (0)

extern POKEREVAL_EXPORT JokerDeck_CardMask JokerDeck_cardMasksTable[JokerDeck_N_CARDS];

extern POKEREVAL_EXPORT uint8              jokerStraightTable[StdDeck_N_RANKMASKS];
 
extern POKEREVAL_EXPORT int JokerDeck_cardToString(int cardIndex, char *outString);
extern POKEREVAL_EXPORT int JokerDeck_stringToCard(char *inString, int *outCard);

#define JokerDeck_cardString(i) GenericDeck_cardString(&JokerDeck, (i))
#define JokerDeck_printCard(i)  GenericDeck_printCard(&JokerDeck, (i))
#define JokerDeck_printMask(m)  GenericDeck_printMask(&JokerDeck, ((void *) &(m)))
#define JokerDeck_maskString(m) GenericDeck_maskString(&JokerDeck, ((void *) &(m)))
#define JokerDeck_numCards(m) GenericDeck_numCards(&JokerDeck, ((void *) &(m)))
#define JokerDeck_maskToString(m, s) GenericDeck_maskToString(&JokerDeck, ((void *) &(m)), (s))

extern POKEREVAL_EXPORT Deck JokerDeck;

#endif


#ifdef DECK_JOKER

#if defined(Deck_N_CARDS)
#include "deck_undef.h"
#endif

#define Deck_N_CARDS      JokerDeck_N_CARDS
#define Deck_MASK         JokerDeck_MASK
#define Deck_RANK         JokerDeck_RANK
#define Deck_SUIT         JokerDeck_SUIT

#define Deck_cardToString JokerDeck_cardToString
#define Deck_maskToString JokerDeck_maskToString
#define Deck_printCard    JokerDeck_printCard
#define Deck_printMask    JokerDeck_printMask
#define Deck_stringToMask JokerDeck_stringToMask

#define Rank_2            JokerDeck_Rank_2 
#define Rank_3            JokerDeck_Rank_3
#define Rank_4            JokerDeck_Rank_4 
#define Rank_5            JokerDeck_Rank_5 
#define Rank_6            JokerDeck_Rank_6 
#define Rank_7            JokerDeck_Rank_7 
#define Rank_8            JokerDeck_Rank_8 
#define Rank_9            JokerDeck_Rank_9 
#define Rank_TEN          JokerDeck_Rank_TEN
#define Rank_JACK         JokerDeck_Rank_JACK
#define Rank_QUEEN        JokerDeck_Rank_QUEEN
#define Rank_KING         JokerDeck_Rank_KING
#define Rank_ACE          JokerDeck_Rank_ACE
#define Rank_FIRST        JokerDeck_Rank_FIRST 
#define Rank_COUNT        JokerDeck_Rank_COUNT

#define Suit_HEARTS       JokerDeck_Suit_HEARTS
#define Suit_DIAMONDS     JokerDeck_Suit_DIAMONDS
#define Suit_CLUBS        JokerDeck_Suit_CLUBS
#define Suit_SPADES       JokerDeck_Suit_SPADES
#define Suit_FIRST        JokerDeck_Suit_FIRST
#define Suit_COUNT        JokerDeck_Suit_COUNT

#define CardMask               JokerDeck_CardMask 
#define CardMask_NOT           JokerDeck_CardMask_NOT
#define CardMask_OR            JokerDeck_CardMask_OR
#define CardMask_XOR           JokerDeck_CardMask_XOR
#define CardMask_AND           JokerDeck_CardMask_AND
#define CardMask_SET           JokerDeck_CardMask_SET
#define CardMask_CARD_IS_SET   JokerDeck_CardMask_CARD_IS_SET
#define CardMask_ANY_SET       JokerDeck_CardMask_ANY_SET
#define CardMask_RESET         JokerDeck_CardMask_RESET
#define CardMask_UNSET         JokerDeck_CardMask_UNSET

#define CardMask_SPADES        JokerDeck_CardMask_SPADES
#define CardMask_HEARTS        JokerDeck_CardMask_HEARTS
#define CardMask_CLUBS         JokerDeck_CardMask_CLUBS
#define CardMask_DIAMONDS      JokerDeck_CardMask_DIAMONDS

#define CardMask_SET_SPADES    JokerDeck_CardMask_SET_SPADES
#define CardMask_SET_HEARTS    JokerDeck_CardMask_SET_HEARTS
#define CardMask_SET_CLUBS     JokerDeck_CardMask_SET_CLUBS
#define CardMask_SET_DIAMONDS  JokerDeck_CardMask_SET_DIAMONDS

#define CurDeck JokerDeck

#endif 

