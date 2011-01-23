/* Showdown.h */
/* Hold'Em Showdown by Steve Brecher <steve@brecher.reno.nv.us> */

#define kVersion "2006Dec01.0"
/*
 *  01Dec06.0   Use dealt array; indexing rather than pointers to access deck.
 *  29Nov06.0	Enumerate boards before enumerating unknown player hole cards.
 *  24Nov06.0	Move inference of 2nd of 2 players results from ResultsOut to Enum2GuysNoFlop.
 *              HandEval internal changes.
 *              HandEval result for straight/straight flush specifies high rank, not 5-bit mask.
 *	08Nov06.0	__int64 (MS-specific) instead of int[4] for Hand_X_Eval argument
 *  16May03.0	Terminate on feof(stdin) so input can be redirected
 *	14Sep02.0	HandEval update.
 *  10Jul02.0	In ResultsOut replace (long)nPots with (long)ceil(nPots-0.5)
 *					to fix output for case 2 players, 2 board cards.
 *	15May02.0	Move global data defs from Showdown.h to Showdown.c.
 *				Remove first (ranks summary) element from hand array.
 *				Fix output justification of "Unknown"/"Unknowns".
 *	14Apr02.0	Windows version:  change output file name to mixed case.
 *  31Oct01.0	Windows version:  build with VC++
 *	30Aug01.0	Mac version: remove reference to closing window from help.
 *	11Dec00.0	Add additional user help.
 *  30Nov97.0   Work around CW Pro2 sscanf bug in Showdown.h:GetNextCard();
 *              Undo change of 07Oct97 and instead test value of d_flop_game
 *                  after #include of HandEval.h
 *  07Oct97.0   #define d_flop_game 1 before #include HandEval.h
 *  23Sep97.0   Fix typo in argument to SIOUXSetTitle.
 *  10Apr97.0   Fix runaway output when players > 4 -- bug introduced 11Dec96.0.
 *  11Dec96.0   Show 6 digits after decimal point.
 *              Output file creator R*ch (BBEdit).
 *  07Aug96.0   Change MaskT to ulong (from short) for PPC RAM access speed.
 *  14May96.0   Eliminate compiler warnings
 *  04May96.0   ANSI portability
 *              fix sense of resultsCreated test in Initialize()
 *  18Feb95.0   relax constraint on number of board cards specified from
 *                  0, 3, or 4 to 0 through 4.
 *  14Feb95.0   move poker logic routines nearer start of this file.
 *  10Feb95.1.0 change name of Enum2GuysNoBoard routine to Enum2GuysNoFlop;
 *              #include <time.h> iff kTimer.
 *  10Feb95.1   special case two players, no unknown, no flop.
 *  10Feb95.0   delete virginDeck -- superfluous;
 *              remove total pots from output and re-format output;
 *              fix typos in following line about 09Feb95.0.
 *  09Feb95.0   Enumerate:  fix logic bug in case 1 (1 unknown player).
 *  08Feb95.2   main: use WithCommas in too-many-enumerations error message,
 *                  thus fixing by removal a bad f format specifier.
 *  08Feb95.1   EnumBoardsNoUnknown: remove superfluous check in outer loop
 *                  for already-used deck card.
 *  08Feb95.0   mPotResults: improved (faster) logic w/r ties.
 *  07Feb95.0   first version without known bugs.
 */

#ifdef __MWERKS__
#ifdef macintosh
#define __Mac__ /* compiled on/for a Macintosh */
#if defined(powerc) || defined (__powerc)
#define __PPC__
#else
#define __68K__
#endif
#endif
#ifdef __INTEL__
#define __Windows__
#endif
#include <stdio.h>
#else
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#endif

#include <stdlib.h>
#include <string.h>
#ifdef THINK_C
#include <console.h>
#endif
#ifdef __MWERKS__
#ifdef __Mac__
#include <SIOUX.h>
#endif
#ifdef __Windows__
#include <WinSIOUX.SB.h>
#endif
#endif
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define k_hands_per_line 4

#define kTimer	1    /* output calculation time if non-zero */

#if kTimer
#ifdef __Mac__
#define __dest_os   __mac_os
#endif
#include <time.h>
#endif

#ifndef __Mac__
typedef unsigned char   Byte;
enum {false, true};
#define nil             NULL
#define kOutFileName    "Showdown.txt"
#else
#define kOutFileName    "Showdown Results"
#define kFileCreator    'R*ch'
#endif

#ifndef __Mac__
#define IndicateWait()
#else
#define IndicateWait() SetCursor(*GetCursor(watchCursor))
#endif

#define kMaxPlayers 22 /* 5 board and 3 burn cards leaves 44 for hole cards */

#include "HandEval.h"
#if !d_flop_game
#error "d_flop_game is zero!"
#endif

enum {kT = 10, kJ, kQ, kK, kA};

void InitDeck(void);
void Enum2GuysNoFlop(void);
void EnumBoardsNoUnknown(void);
void EnumBoards(void);
void Enumerate(void);
void PrintCard(FILE *f, Hand_T *h);
void General_Help(void);
int GetNextCard(char *string, int *offset, Hand_T *h);
void ReadLine(char *s, int max);
Boolean GetResponse(char *prompt, char *s, int max, char *help_text);
Boolean GetHoleCards(int *nPlayers);
Boolean GetBoard(int *nBoardCards, Hand_T *board);
Boolean GetDeadCards(int *nDeadCards);
Boolean GetNbrUnknown(int *nUnknown);
char *WithCommas(double n, char *s);
Boolean UserConfirm(double nDeals);
void Initialize(void);
void ResultsOut(void);
double Factorial(double n);
double Combos(int chooseFrom, int choose);
double NbrEnumerations(void);
