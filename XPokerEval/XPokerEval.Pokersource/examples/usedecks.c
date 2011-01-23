/* $Id: usedecks.c 1024 2006-04-22 20:49:41Z loic $
   Example showing how to use different decks and evaluation rule sets.

   Michael Maurer, Apr 2002

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
#include <string.h>
#include <stdlib.h>

#define TEST_STD
#define TEST_JOKER
#define TEST_ASTUD
#define TEST_OMAHA8

#include "poker_defs.h"

#ifdef TEST_STD
#include "inlines/eval.h"
#include "inlines/eval_low.h"
#include "inlines/eval_low8.h"
#include "deck_std.h"
#include "rules_std.h"

int 
testStdDeck(const char *handstr) {
  StdDeck_CardMask cards;
  int ncards = 0;
  int c = 0;
  HandVal hival;
  LowHandVal loval;
  LowHandVal lo8val;
  char str[80];
  char *p;

  printf("\nStandard Deck: %s\n", handstr);
  StdDeck_CardMask_RESET(cards);
  strcpy(str, handstr);
  p = strtok(str, " ");
  do {
    if (DstringToCard(StdDeck, p, &c) == 0)
      goto error;
    if (!StdDeck_CardMask_CARD_IS_SET(cards, c)) {
      StdDeck_CardMask_SET(cards, c);
      ++ncards;
    };
  } while ((p = strtok(NULL, " ")) != NULL);

  hival = StdDeck_StdRules_EVAL_N(cards, ncards);
  printf("%s: %d: ", DmaskString(StdDeck, cards), hival);
  StdRules_HandVal_print(hival);
  printf("\n");

  loval = StdDeck_Lowball_EVAL(cards, ncards);
  printf("%s (lowball): %d: ", DmaskString(StdDeck, cards), loval);
  LowHandVal_print(loval);
  printf("\n");

  lo8val = StdDeck_Lowball8_EVAL(cards, ncards);
  printf("%s (low8): %d: ", DmaskString(StdDeck, cards), lo8val);
  LowHandVal_print(lo8val);
  printf("\n");

  return 0;

 error:
  printf("ERROR\n");
  return 1;
}

#endif

#ifdef TEST_JOKER
#include "inlines/eval_joker.h"
#include "inlines/eval_joker_low.h"
#include "inlines/eval_joker_low8.h"
#include "deck_joker.h"
#include "rules_joker.h"

int 
testJokerDeck(const char *handstr) {
  JokerDeck_CardMask cards;
  int ncards = 0;
  int c = 0;
  HandVal hival;
  LowHandVal loval;
  LowHandVal lo8val;
  char str[80];
  char *p;

  printf("\nJoker Deck: %s\n", handstr);
  JokerDeck_CardMask_RESET(cards);
  strcpy(str, handstr);
  p = strtok(str, " ");
  do {
    if (JokerDeck.stringToCard(p, &c) == 0)
      goto error;
    if (!JokerDeck_CardMask_CARD_IS_SET(cards, c)) {
      JokerDeck_CardMask_SET(cards, c);
      ++ncards;
    };
  } while ((p = strtok(NULL, " ")) != NULL);

  hival = JokerDeck_JokerRules_EVAL_N(cards, ncards);
  printf("%s: %d: ", DmaskString(JokerDeck, cards), hival);
  JokerRules_HandVal_print(hival);
  printf("\n");

  loval = JokerDeck_Lowball_EVAL(cards, ncards);
  printf("%s (lowball): %d: ", DmaskString(JokerDeck, cards), loval);
  LowHandVal_print(loval);
  printf("\n");

  lo8val = JokerDeck_Lowball8_EVAL(cards, ncards);
  printf("%s (low8): %d: ", DmaskString(JokerDeck, cards), lo8val);
  LowHandVal_print(lo8val);
  printf("\n");

  return 0;

 error:
  printf("ERROR\n");
  return 1;
}
#endif

#ifdef TEST_ASTUD

#include "deck_astud.h"
#include "rules_astud.h"
#include "inlines/eval_astud.h"	/* must come after above!? */

int 
testAStudDeck(const char *handstr) {
  AStudDeck_CardMask cards;
  int ncards = 0;
  int c = 0;
  HandVal hival;
  char str[80];
  char *p;

  printf("\nAsian Stud Deck: %s\n", handstr);
  AStudDeck_CardMask_RESET(cards);
  strcpy(str, handstr);
  p = strtok(str, " ");
  do {
    if (AStudDeck.stringToCard(p, &c) == 0)
      goto error;
    if (!AStudDeck_CardMask_CARD_IS_SET(cards, c)) {
      AStudDeck_CardMask_SET(cards, c);
      ++ncards;
    };
  } while ((p = strtok(NULL, " ")) != NULL);

  hival = AStudDeck_AStudRules_EVAL_N(cards, ncards);
  printf("%s: %d: ", DmaskString(AStudDeck, cards), hival);
  AStudRules_HandVal_print(hival);
  printf("\n");

  return 0;

 error:
  printf("ERROR\n");
  return 1;
}
#endif

#ifdef TEST_OMAHA8
#include "inlines/eval_omaha.h"
#include "deck_std.h"
#include "rules_std.h"

int 
testOmaha8(const char *holestr, const char *boardstr) {
  StdDeck_CardMask hole;
  StdDeck_CardMask board;
  int nhole;
  int nboard;
  int c;
  int ret;
  HandVal hival = 0;
  LowHandVal loval = 0;
  char str[80], hstr[80], bstr[80];
  char *p;

  printf("\nOmaha Hi/Lo8: %s | %s\n", holestr, boardstr);

  nhole = 0;
  StdDeck_CardMask_RESET(hole);
  strcpy(str, holestr);
  p = strtok(str, " ");
  do {
    if (DstringToCard(StdDeck, p, &c) == 0)
      goto error;
    if (!StdDeck_CardMask_CARD_IS_SET(hole, c)) {
      StdDeck_CardMask_SET(hole, c);
      ++nhole;
    };
  } while ((p = strtok(NULL, " ")) != NULL);

  nboard = 0;
  StdDeck_CardMask_RESET(board);
  strcpy(str, boardstr);
  p = strtok(str, " ");
  do {
    if (DstringToCard(StdDeck, p, &c) == 0)
      goto error;
    if (!StdDeck_CardMask_CARD_IS_SET(board, c)) {
      StdDeck_CardMask_SET(board, c);
      ++nboard;
    };
  } while ((p = strtok(NULL, " ")) != NULL);

  ret = StdDeck_OmahaHiLow8_EVAL(hole, board, &hival, &loval);
  strcpy(hstr, DmaskString(StdDeck, hole));
  strcpy(bstr, DmaskString(StdDeck, board));
  printf("%s | %s:\n", hstr, bstr);
  if (ret == 0) {
    printf("  HI %d: ", hival);
    StdRules_HandVal_print(hival);
    printf("\n  LO %d: ", loval);
    LowHandVal_print(loval);
    printf("\n");
  } else {
    printf("ERROR %d\n", ret);
  }
  return 0;

 error:
  printf("ERROR\n");
  return 1;
}

#endif

int 
main(int argc, char **argv) {
#ifdef TEST_STD
  printf("\n================= STANDARD DECK ===================\n");
  testStdDeck("Ac Ad Kc 9d 8s");
  testStdDeck("2c 2d Kc 9d 8s");
  testStdDeck("Ac 7d 8c 9h Ts");
  testStdDeck("Ac 2s 3d 4h 9c");
  testStdDeck("4h 5h 6d 7c 8d");
  testStdDeck("Ac 2s 3d 4h 5s");
  testStdDeck("Ac 2s 3d 4h 7s 3h 4d");
  testStdDeck("2s 3d 4h 5s 6d 3h 4d");

  testStdDeck("Ks Kh Kd 7s 7h 2d 3c");
  testStdDeck("Ks Kh Kd 7s 7h 3h 3c");	/* low eval is wrong */
  testStdDeck("Ks Kh 7s 7h 7d 3s 3h");	/* low eval is wrong */
  testStdDeck("Ks Kh 7s 7h 3s 3h 3d");	/* low eval is wrong */
  testStdDeck("Ks Kh 7s 7h 3s 3h 2c");
  testStdDeck("Ks Kh Kd Kc 4s 4h 2c");
  testStdDeck("Ks Kh Kd Kc 4s 4h 4d");
  testStdDeck("Ks Kh Kd Kc 4s 4h 7c");
  testStdDeck("4s 4h 4d 4c Ks Kh 2c");
  testStdDeck("4s 4h 4d 4c Ks Kh 7c");
  testStdDeck("4s 4h 4d 4c Ks Kh Kd");	/* low eval is wrong */
#endif

#ifdef TEST_JOKER
  printf("\n================= JOKER DECK ===================\n");
  testJokerDeck("Ac Ad Kc 9d 8s");
  testJokerDeck("2c 2d Kc 9d 8s");
  testJokerDeck("Ac 7d 8c 9h Ts");
  testJokerDeck("Ac 2s 3d 4h 9c");
  testJokerDeck("4h 5h 6d 7c 8d");
  testJokerDeck("Ac 2s 3d 4h 5s");
  testJokerDeck("xx 2s 3s 4c 5c");
  testJokerDeck("xx Ac 3c 4c 7c");
  testJokerDeck("Kc Ac 3c 4c 7c");
  testJokerDeck("xx Jc 3c 4c 7c");
  testJokerDeck("xx Ac As 2d 2h");
  testJokerDeck("xx Ac As Ad Ah");
  testJokerDeck("Ks Kh Kd As Ah");
#endif

#ifdef TEST_ASTUD
  printf("\n================= ASIAN STUD DECK ===================\n");
  testAStudDeck("Ac Ad Kc 9d 8s");
  testAStudDeck("Ac 7d 8c 9h Ts");
  testAStudDeck("Ac Qc Tc 8c 7c");
  testAStudDeck("Ac Ad As Th Td");
#endif

#ifdef TEST_OMAHA8
  printf("\n================= OMAHA HI/LOW8 ===================\n");
  testOmaha8("Ac 2c Jd Th", "As Kc Qc 8d 2d");
  testOmaha8("Ac 2c Jd 8h", "As Kc Qc Jh Td");
  testOmaha8("3c 2c Jd 8h", "As Kc Qc Jh Td");
  testOmaha8("Qs 4h 4d 4s", "As Ks 8h 9d 2s");
  testOmaha8("Qs 4h 4d Qd", "As Ks 8s 9s 2s");
  testOmaha8("Td Tc Ad 9c", "As Ts 8s 8h 4d");
  testOmaha8("Td 8c Ad 9c", "As Ts 8s 8h 4d");
  testOmaha8("Ah 2h 3h 5h", "As Ac 8s 8h 4d");
  testOmaha8("Ah 2h 3h 4h", "As Ac 8s 8h 4d");
  testOmaha8("8c Jc Jd Th", "As Kc Qc 8d 2d");
  testOmaha8("Ac 2c Jd Th", "3d 5h 8d Tc Ts");
  testOmaha8("Ac 3c 4d Th", "3d 5h 8d Tc Ts");
  testOmaha8("Ac 3c 5d 8h", "3d 5h 8d Ad Ts");
  testOmaha8("Ad 2d Th Td", "Ac 2c 3d 4h 5s");
  testOmaha8("4d 5d Th Td", "Ac 2c 3d 4h 5s");
  testOmaha8("Ad 2d Th Td", "5h 7h 8d Ac 2c");
#endif

  return 0;
}
