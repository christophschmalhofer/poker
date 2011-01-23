/* HandEval.h           by Steve Brecher */

/*
 *  24Nov06.0   HandEval paramenter is four 13-bit fields in low order 52 bits.
 *              Rank masks 13 low-order bits instead of 14 bits -- kArraySize reduced by half.
 *              HandEval result for straight/straight flush specifies high rank, not 5-bit mask.
 *	08Nov06.0	__int64 (MS-specific) instead of int[4] for Hand_X_Eval argument
 *	14Sep02.0	Minor code edits for clarity.
 *				Add Omaha8_Low_Eval function.
 *  10Jul02.0	Fix SetStraight (broken in unreleased 15May02.0).
 *	15May02.0	Use straightValue (formerly theStraight) nonzero in place of
 *					former Boolean straight[].
 *				Two pair eval result always includes kicker regardless
 *					of d_flop_game.
 *				Change type of nbrOfRanks[] and nbrOfRanksGE5[] to int, hoping
 *					for faster access than for char.
 *				Remove first (ranks summary) element from hand array.
 *	26Apr02.0	Finer decision granularity in mStraightAndOrFlush macro
 *					and likewise in straight/flush logic in Hand_5_Eval.
 *	14Apr02.0	Add Hand_6_Eval function.
 *				Change name of d_lang macro to d_prefix.
 *  04Aug96.0   AND only two suit masks to get quads bit in HandEval.
 *  14May96.0   Eliminate compiler warnings
 *  23May95.0   fix case of three pair and singleton to take kicker from higher
 *                  of 3rd-ranking pair or singleton instead of always from
 *                  3rd-ranking pair
 *              introduce Boolean nbrOfRanksGE5 array to help testing for flush;
 #              use bits 1 to 13 instead of 2 to 14 in masks;
 *              make lookup table elements longs instead of shorts;
 *              misc. minor optimizations, some due to Cliff Matthews et al;
 *              remove "unsigned" from Mask_T -- big win in code gen;
 *              remove total pots from output and re-format output;
 *              InitTables:  remove knowledge of types from calls to calloc;
 */

#ifndef d_HandEval_h    /* { */
#define d_HandEval_h
#pragma once

/* true for Asian stud: */
#ifndef d_asian
#define d_asian 0
#endif

/*
 *  If d_flop_game is nonzero then evaluation result includes bits for
 *  non-playing kickers with quads or trips nor non-playing pair with full
 *  house.  This costs a little execution time.  I.e., set to zero for draw
 *  poker, stud poker, Chinese Poker; set to nonzero for hold 'em, Omaha,
 *  and other common-card games.
 */
#ifndef d_flop_game
#define d_flop_game 1
#endif

/* Function which allocates zero-filled bytes on the heap: */
/* optional for Macintosh: */
/* #define d_ram_alloc(quantity, size) NewPtrClear((quantity) * (size)) */
/* Portable ANSI C: */
#include <stdlib.h>
#define d_ram_alloc(quantity, size) calloc(quantity, size)

/* function prefix */
#ifdef d_is_DLL
/* We are making a DLL for Windows... */
#define d_prefix __declspec(dllexport)
#else
#define d_prefix
/* For making a library to be used with Macintosh Pascal: */
/* #define d_prefix pascal */
#endif

/*
	 * A hand is 0 or more cards represented in four 13-bit masks, one
	 * mask per suit, in the low-order 52 bits of a long (64 bits). In
	 * each mask, bit 0 set (0x0001) for a deuce, ..., bit 12 set
	 * (0x1000) for an ace. Each mask denotes the ranks present in one
	 * of the suits.
 */

#define Encode(rank, suit) (1i64 << (suit*13 + rank - 2))

enum {kClubs, kDiamonds, kHearts, kSpades, kNbrOfMasks};
/*
 *  Actually the order of the suit elements of the array is immaterial
 *	and this module uses only the k_Nbr_of_masks constant.
 */
#define k_ace_mask      (0x00001000)
#define k_deuce_mask    (0x00000001)

enum {NO_PAIR, PAIR, TWO_PAIR, THREE_OF_A_KIND,
        STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH};

typedef unsigned long Eval_T; /*  Evaluation result in 32 bits = 0VTBMMMM
	/*
	 * Different functions are called for high and for lowball evaluation.  The
	 * result format below is the same except:  For lowball evaluation, as for
	 * wheels in high evaluation, Ace is rank 1 and its mask bit is the LS bit;
	 * otherwise Ace is rank 14, mask bit 0x1000, and the deuce's mask bit is
     * the LS bit.
	 *
	 * For normal evaulation if results R1 > R2, hand 1 beats hand 2;
	 * for lowball evaluation if results R1 > R2, hand 2 beats hand 1.
	 *
	 * Evaluation result in 32 bits = 0x0VTBKKKK:
	 * 
	 * V nybble = value code (NO_PAIR..STRAIGHT_FLUSH)
	 * T nybble = rank (2..14) of top pair for two pair, 0 otherwise
	 * B nybble = rank (1..14) of trips (including full house trips) or quads,
	 *  			or rank of high card (5..14) in straight or straight flush,
	 *              or rank of bottom pair for two pair (hence the symbol "B"),
	 *              or rank of pair for one pair,
	 *              or 0 otherwise
	 * KKKK mask = 16-bit mask with...
	 *              5 bits set for no pair or (non-straight-)flush
	 *              3 bits set for kickers with pair,
	 */
#if d_flop_game
	/*
	 *              2 bits set for kickers with trips,
	 *              1 bit set for pair with full house or kicker with quads
   */
#endif
	/*				1 bit set for kicker with two pair
	 *              or 0 otherwise
	 */
#define k_No8Low	0x0FFFFFFF;	/* eval result meaning no 8-or-better low */

typedef __int64	Hand_T;

#ifndef true
typedef unsigned char Boolean;
#define false 0
#define true 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if d_asian
d_prefix Boolean Init_Asian_Eval(void);   /* returns false if heap allocation
                                            fails */
d_prefix Eval_T Asian_5_Eval(Hand_T hand);
#else
d_prefix Boolean Init_Hand_Eval(void);    /* returns false if heap allocation
                                            fails */
d_prefix Eval_T Hand_7_Eval(Hand_T hand);
d_prefix Eval_T Hand_6_Eval(Hand_T hand);
d_prefix Eval_T Hand_5_Eval(Hand_T hand);
d_prefix Eval_T Hand_5_Ato5Lo_Eval(Hand_T hand);
d_prefix Eval_T Hand_Razz_Eval(Hand_T hand);
d_prefix Eval_T Hand_8Low_Eval(Hand_T hand);
d_prefix Eval_T Ranks_8Low_Eval(int ranks);
d_prefix Eval_T No8LowValue(void);		/*	returns the constant ..._Eval() result
											that means no 8-or-better low */
d_prefix Eval_T Omaha8_Low_Eval(int twoHoleRanks, int boardRanks);

#endif

/* Utilities */
/* Init_..._Eval must have been previously called! */
/* mask argument must not exceed 0x1FC0 */
d_prefix int Hi_Card_Mask(short mask);    /* result is mask with all bits except
												 highest reset */
d_prefix int Number_Of_Ranks(short mask);    /* number of bits set in mask */
d_prefix int Rank_Of_Hi_Card(short mask);    /* rank (0..kA) of highest bit set
												in mask */
d_prefix void Decode(Hand_T hand, int *rank, int *suit);

#ifdef __cplusplus
}
#endif

#endif  /* } */
