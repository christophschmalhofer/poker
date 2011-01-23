/*
 *  hcmpn.c: a program to compare n pairs of hold'em hole cards at any
 *           point of the game (pre-flop, on the flop, turn or river).
 *              
 *  Example:
 *
 *      hcmpn  tc ac  3h ah  2h 2d -- 8c 6h 7h
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
#include <stdlib.h>
#include <string.h>

#include "poker_defs.h"
#include "inlines/eval.h"

#define MAX_PLAYERS 22

int gNCommon, gNDead, gNPlayers;
CardMask gDeadCards, gCommonCards, gPlayerCards[MAX_PLAYERS];


static void
parseArgs(int argc, char **argv) {
  int i, seenSep = 0, cardCount = 0, c;

  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-d") == 0) {
	if (++i == argc) goto error;
        if (StdDeck_stringToCard(argv[i], &c) == 0)
          goto error;
        if (!CardMask_CARD_IS_SET(gDeadCards, c)) {
          ++gNDead;
          StdDeck_CardMask_SET(gDeadCards, c);
        };
      } 
      else if (!strcmp(argv[i], "--"))
        seenSep = 1;
      else
        goto error;
    } else {
      if (StdDeck_stringToCard(argv[i], &c) == 0)
        goto error;
      if (seenSep) {
        StdDeck_CardMask_SET(gCommonCards, c);
        ++gNCommon;
      }
      else {
        int i = cardCount / 2;
        StdDeck_CardMask_SET(gPlayerCards[i], c);
        gNPlayers = i+1;
        ++cardCount;
      };
    }
  }
  if (gNPlayers < 2) goto error;
  if (gNCommon > 5) goto error;

  return;

 error:
  fprintf(stderr, "Usage: hcmp2 [ -d dead-card ] p1-cards .. pn-cards [ -- common-cards ]\n");
  exit(0);
}


int main( int argc, char *argv[] )
{
  CardMask deadCards, cards, playerCards[MAX_PLAYERS], pCards;
  HandVal handval[MAX_PLAYERS], maxHand;
  int i;
  unsigned long winCount[MAX_PLAYERS], loseCount[MAX_PLAYERS], 
    tieCount[MAX_PLAYERS], handCount=0, nWinners;
  float ev[MAX_PLAYERS];

  CardMask_RESET(gDeadCards);
  CardMask_RESET(gCommonCards);
  
  for (i=0; i<MAX_PLAYERS; i++) {
    CardMask_RESET(gPlayerCards[i]);
    winCount[i] = 0;
    tieCount[i] = 0;
    loseCount[i] = 0;
    ev[i] = 0;
  };
  parseArgs(argc, argv);
  
  deadCards = gDeadCards;
  for (i=0; i<gNPlayers; i++) {
    CardMask_OR(playerCards[i], gPlayerCards[i], gCommonCards);
    CardMask_OR(deadCards, deadCards, playerCards[i]);
  };

  ENUMERATE_N_CARDS_D(cards, 5-gNCommon, deadCards, 
                      {
                        ++handCount;
                        nWinners = 0;
                        maxHand = HandVal_NOTHING;
                        for (i=0; i<gNPlayers; i++) {
                          CardMask_OR(pCards, playerCards[i], cards);
                          handval[i] = Hand_EVAL_N(pCards, 7);
                          if (handval[i] > maxHand) {
                            nWinners = 1;
                            maxHand = handval[i];
                          }
                          else if (handval[i] == maxHand) 
                            ++nWinners;
                        };

                        for (i=0; i<gNPlayers; i++) {
                          if (handval[i] == maxHand) {
                            if (nWinners == 1) {
                              ++winCount[i];
                              ev[i] += 1.0;
                            }
                            else {
                              ++tieCount[i];
                              ev[i] += (1.0 / nWinners);
                            };
                          }
                          else
                            ++loseCount[i];
                        }
                      }
                      );

  printf("%ld boards", handCount);
  if (gNCommon > 0) 
    printf(" containing %s ", Deck_maskString(gCommonCards));
  if (gNDead) 
    printf(" with %s removed ", Deck_maskString(gDeadCards));
  printf("\n");

  printf("  cards      win  %%win       loss  %%lose       tie  %%tie      EV\n");
  for (i=0; i<gNPlayers; i++) 
    printf("  %s  %7ld %6.2f   %7ld %6.2f   %7ld %6.2f     %6.4f\n", 
           Deck_maskString(gPlayerCards[i]), 
           winCount[i], 100.0*winCount[i]/handCount, 
           loseCount[i], 100.0*loseCount[i]/handCount, 
           tieCount[i], 100.0*tieCount[i]/handCount, 
           ev[i] / handCount);

  return 0;
  }
