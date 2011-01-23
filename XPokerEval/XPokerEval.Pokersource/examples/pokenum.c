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
/* $Id: pokenum.c 1773 2006-10-12 11:34:41Z loic $
   Example showing how to calculate pot equity using the enumeration functions.
   See also hcmp2.c and hcmpn.c for slightly lower level examples.

Note: games that use the joker are not yet implemented, though listed below.

Typical usage: 
$ pokenum [-mc niter] [-t] [-O]
          [-h|-h8|-o|-o8|-7s|-7s8|-7snsq|-r|-5d|-5d8|-5dnsq|-l|-l27]
          <pocket1> - <pocket2> - ... [ -- <board> ] [ / <dead> ]
      -t     terse output (one line, list of EV values)
      -O     compute and output detailed relative rank ordering histogram
      -mc    sample monte-carlo style instead of full enumeration
      -h     holdem hi (default)
      -h8    holdem hi/lo 8-or-better
      -o     omaha hi
      -o8    omaha hi/lo 8-or-better
      -7s    7-card stud hi
      -7s8   7-card stud hi/lo 8-or-better
      -7snsq 7-card stud hi/lo no stinking qualifier
      -r     7-card stud ace-to-5 low (razz)
      -5d    5-card draw hi (with joker)
      -5d8   5-card draw hi/lo 8-or-better (with joker)
      -5dnsq 5-card draw hi/lo no stinking qualifier (with joker)
      -l     5-card draw ace-to-5 lowball (with joker)
      -l27   5-card draw deuce-to-seven lowball

Streaming usage:
$ pokenum -i
followed by lines on stdin, one enumeration request per line, for example:
  -t -h Ac 7c - 5s 4s - Ks Kd
  -t -h Ac 7c - 5s 4s - Ks Kd -- 7h 2c 3h
  -t -h Ac 7c - 5s 4s - Ks Kd -- 7h 2c 3h 4d
  -t -mc 10000 -l27 5h 4h 3h / 5s Qd - 9s 8h 6d / Ks Kh

Use '-' between each player (optional in flop games).
Use '--' before the board (if any).
Use '/' before dead cards (can appear within a hand for discards).
Use 'Xx' for the joker.

Examples:

$ pokenum -h Ac 7c - 5s 4s - Ks Kd
$ pokenum -h Ac 7c 5s 4s Ks Kd
$ pokenum -h Ac 7c 5s 4s Ks Kd -- 7h 2c 3h

$ pokenum -o As Kh Qs Jh - 8h 8d 7h 6d
$ pokenum -o As Kh Qs Jh 8h 8d 7h 6d
$ pokenum -o As Kh Qs Jh 8h 8d 7h 6d -- 8s Ts Jc

$ pokenum -7s As Ah Ts Th 8h 8d - Kc Qc Jc Td 3c 2d
$ pokenum -7s As Ah Ts Th 8h 8d - Kc Qc Jc Td 3c 2d / 5c 6c 2s Jh

$ pokenum -l 7h 5s 3d Xx / Kd - 9s 8h 6d 4c / 8c
$ pokenum -l27 5h 4h 3h 2h / 5s - 9s 8h 6d 4c / Kd
$ pokenum -mc 10000 -l27 5h 4h 3h / 5s Qd - 9s 8h 6d / Ks Kh

   Michael Maurer, Apr 2002
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "poker_defs.h"
#include "enumdefs.h"

static int
parseArgs(int argc, char **argv,
          enum_game_t *game, enum_sample_t *enumType, int *niter,
          StdDeck_CardMask pockets[], StdDeck_CardMask *board,
          StdDeck_CardMask *dead, int *npockets, int *nboard,
          int *orderflag, int *terse) {
  /* we have a type problem: we define the masks here as
     StdDeck_CardMask, which makes it impossible to hold jokers.
     we need to redesign some of the deck typing to make this work... */
  enum_gameparams_t *gameParams = enumGameParams(game_holdem);
  enum { ST_OPTIONS, ST_POCKET, ST_BOARD, ST_DEAD } state;
  int ncards;
  int card;
  int i;

  state = ST_OPTIONS;
  *npockets = *nboard = ncards = 0;
  *terse = 0;
  *orderflag = 0;
  *game = game_holdem;
  *enumType = ENUM_EXHAUSTIVE;
  StdDeck_CardMask_RESET(*dead);
  StdDeck_CardMask_RESET(*board);
  for (i=0; i<ENUM_MAXPLAYERS; i++)
    StdDeck_CardMask_RESET(pockets[i]);
  while (++argv, --argc) {
    if (state == ST_OPTIONS) {
      if (argv[0][0] != '-') {
        state = ST_POCKET;
        argv--; argc++;
      } else {
        if (strcmp(*argv, "-mc") == 0) {
          *enumType = ENUM_SAMPLE;
          if (argc < 1)
            return 1;
          *niter = strtol(argv[1], NULL, 0);
          if (*niter <= 0 || errno != 0)
            return 1;
          argv++; argc--;                       /* put card back in list */
        } else if (strcmp(*argv, "-t") == 0) {
          *terse = 1;
        } else if (strcmp(*argv, "-O") == 0) {
          *orderflag = 1;
        } else if (strcmp(*argv, "-h") == 0) {
          *game = game_holdem;
        } else if (strcmp(*argv, "-h8") == 0) {
          *game = game_holdem8;
        } else if (strcmp(*argv, "-o") == 0) {
          *game = game_omaha;
        } else if (strcmp(*argv, "-o8") == 0) {
          *game = game_omaha8;
        } else if (strcmp(*argv, "-7s") == 0) {
          *game = game_7stud;
        } else if (strcmp(*argv, "-7s8") == 0) {
          *game = game_7stud8;
        } else if (strcmp(*argv, "-7snsq") == 0) {
          *game = game_7studnsq;
        } else if (strcmp(*argv, "-r") == 0) {
          *game = game_razz;
        } else if (strcmp(*argv, "-5d") == 0) {
          *game = game_5draw;
        } else if (strcmp(*argv, "-5d8") == 0) {
          *game = game_5draw8;
        } else if (strcmp(*argv, "-5dnsq") == 0) {
          *game = game_5drawnsq;
        } else if (strcmp(*argv, "-l") == 0) {
          *game = game_lowball;
        } else if (strcmp(*argv, "-l27") == 0) {
          *game = game_lowball27;
        } else {                                /* unknown option switch */
          return 1;
        }
        if ((gameParams = enumGameParams(*game)) == NULL)
          return 1;
      }
      
    } else if (state == ST_POCKET) {
      if (strcmp(*argv, "-") == 0) {            /* player delimiter */
        if (ncards > 0) {                       /* is a player pending? */
          if (ncards < gameParams->minpocket)   /* too few pocket cards */
            return 1;
          (*npockets)++;
          ncards = 0;
        }
        state = ST_POCKET;
      } else if (strcmp(*argv, "--") == 0) {    /* board prefix */
        state = ST_BOARD;
      } else if (strcmp(*argv, "/") == 0) {     /* dead card prefix */
        state = ST_DEAD;
      } else {
        if (*npockets >= ENUM_MAXPLAYERS)           /* too many players */
          return 1;
        if (DstringToCard(StdDeck, *argv, &card) == 0) /* parse error */
          return 1;
        if (StdDeck_CardMask_CARD_IS_SET(*dead, card)) /* card already seen */
          return 1;
        StdDeck_CardMask_SET(pockets[*npockets], card);
        StdDeck_CardMask_SET(*dead, card);
        ncards++;
        if (ncards == gameParams->maxpocket) {  /* implicit player delimiter */
          (*npockets)++;
          ncards = 0;
        }
      }
      
    } else if (state == ST_BOARD) {
      if (strcmp(*argv, "/") == 0) {            /* dead card prefix */
        state = ST_DEAD;
      } else {
        if (DstringToCard(StdDeck, *argv, &card) == 0) /* parse error */
          return 1;
        if (StdDeck_CardMask_CARD_IS_SET(*dead, card)) /* card already seen */
          return 1;
        if (*nboard >= gameParams->maxboard) /* too many board cards */
          return 1;
        StdDeck_CardMask_SET(*board, card);
        StdDeck_CardMask_SET(*dead, card);
        (*nboard)++;
      }
      
    } else if (state == ST_DEAD) {
      if (strcmp(*argv, "-") == 0) {            /* player delimiter */
        if (ncards > 0) {                       /* is a player pending? */
          if (ncards < gameParams->minpocket)   /* too few pocket cards */
            return 1;
          (*npockets)++;
          ncards = 0;
        }
        state = ST_POCKET;
      } else {
        if (DstringToCard(StdDeck, *argv, &card) == 0) /* parse error */
          return 1;
        if (StdDeck_CardMask_CARD_IS_SET(*dead, card)) /* card already seen */
          return 1;
        StdDeck_CardMask_SET(*dead, card);
      }
    }
  }
  if (ncards > 0) {                             /* is a player pending? */
    if (ncards < gameParams->minpocket)         /* too few pocket cards */
      return 1;
    (*npockets)++;
    ncards = 0;
  }
  if (*npockets == 0)                           /* no players seen */
    return 1;
  return 0;
}

#define MAX_ARGS 100
#define BUF_LEN 1000

int 
main(int argc, char **argv) {
  enum_game_t game;
  enum_sample_t enumType;
  int niter = 0, npockets, nboard, err, terse, orderflag;
  StdDeck_CardMask pockets[ENUM_MAXPLAYERS];
  StdDeck_CardMask board;
  StdDeck_CardMask dead;
  enum_result_t result;
  int fromStdin;

  fromStdin = (argc == 2 && !strcmp(argv[1], "-i"));
  if (fromStdin)
    argv = (char **) malloc(MAX_ARGS * sizeof(char *));
  do {
    err = 0;
    enumResultClear(&result);
    if (fromStdin) {	/* read one line from stdin, split into argv/argc */
      char buf[BUF_LEN], *p;
      if (fgets(buf, sizeof(buf), stdin) == NULL)
        break;
      argc = 0;
      argv[argc++] = "pokenum";
      p = strtok(buf, " \t\r\n");
      while (p != NULL) {
        argv[argc++] = p;
        p = strtok(NULL, " \t\r\n");
      }
    }
    if (parseArgs(argc, argv, &game, &enumType, &niter,
                  pockets, &board, &dead, &npockets, &nboard,
                  &orderflag, &terse)) {
      if (fromStdin) {
        printf("ERROR\n");
      } else {
        printf("single usage: %s [-t] [-O] [-mc niter]\n", argv[0]);
        printf("\t[-h|-h8|-o|-o8|-7s|-7s8|-7snsq|-r|-5d|-5d8|-5dnsq|-l|-l27]\n");
        printf("\t<pocket1> - <pocket2> - ... [ -- <board> ] [ / <dead> ] ]\n");
        printf("streaming usage: %s -i < argsfile\n", argv[0]);
      }
      err = 1;
    } else {
      if (enumType == ENUM_EXHAUSTIVE) {
        err = enumExhaustive(game, pockets, board, dead, npockets, nboard,
                             orderflag, &result);
      } else if (enumType == ENUM_SAMPLE) {
        err = enumSample(game, pockets, board, dead, npockets, nboard, niter,
                         orderflag, &result);
      } else {
        err = 1;
      }
      if (err) {
        if (fromStdin)
          printf("ERROR\n");
        else
          printf("enumeration function failed err=%d\n", err);
      } else {
        if (terse)
          enumResultPrintTerse(&result, pockets, board);
        else
          enumResultPrint(&result, pockets, board);
      }
    }
    enumResultFree(&result);
    fflush(stdout);
  } while (fromStdin);
  return err;
}
