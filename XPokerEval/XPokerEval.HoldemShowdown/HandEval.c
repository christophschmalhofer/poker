/* HandEval.c           by Steve Brecher    version 14Sep02.0 */

#include "HandEval.h"

#define kBotRankShift   (16)
#define kTopRankShift   (kBotRankShift+4)
#define kValueShift     (kTopRankShift+4)

#define Value(a)        ((Mask_T)(a) << kValueShift)
#define kAce            (14)

#if d_flop_game
#define with(kickers) | (kickers)
#else
#define with(kickers)
#endif

/* Pointers to heap blocks for which subscript is bit mask of card ranks
   in hand: */
typedef int	Mask_T;

#define kArraySize (0x1FC0 + 1)	/* all combos of up to 7 of LS 13 bits on */

static Mask_T   *straightValue; /* Value(STRAIGHT) | (straight's high card rank (5..14) << kBotRankShift); 0 if no straight */
static int	    *nbrOfRanks;    /* count of bits set */

static Mask_T   *hiTopRankTWO_PAIR;			/* Value(TWO_PAIR) | ((rank (2..kA) of the
													highest bit set) << kTopRankShift) */
static Mask_T   *hiBotRank;					/* (rank (2..kA) of the highest bit set)
												<< kBotRankShift */
static Mask_T   *hiRankMask;    /* all bits except highest reset */
static Mask_T   *hi2RanksMask;  /* all bits except highest 2 reset */
static Mask_T   *hi3RanksMask;  /* all bits except highest 3 reset */
static Mask_T   *hi5RanksMask;  /* all bits except highest 5 reset */

static Mask_T   *lo5RanksMask;  /* all bits except lowest 5 8-or-better reset;
								   0 if not at least 5 8-or-better bits set */
static Mask_T   *lo3RanksMask;	/* all bits except lowest 3 8-or-better reset;
								   0 if not at least 3 8-or-better bits set */
static Eval_T   *loEvalOrNo8Low; /* 5 bits set in LS 8 bits, or constant
									indicating no 8-or-better low */

#if d_asian /* ranks 2-6 removed from deck; 5-card stud */
#define d_wheel 0x000011E0 /* AT987 */
#else /* { */
#define d_wheel 0x0000100F /* A5432 */

/* x is the ranks mask for a suit; N is the number of cards available */
/* These macros assume availablility of scratchpad variables: Mask_T i,j */
#define mFlushOrStraightFlush(x,N)                          \
	if ((j += nbrOfRanks[x]) > (N-5)) {                     \
		if (nbrOfRanks[x] >= 5)                             \
			if (!(i = straightValue[x]))					\
				return Value(FLUSH) | hi5RanksMask[x];		\
			else                                            \
				return (Value(STRAIGHT_FLUSH) - Value(STRAIGHT)) + i; }
/* This macro uses the variables: Mask_T ranks, c, d, h, s */
#define mStraightAndOrFlush(N)                              \
	j = 0;                                                  \
    mFlushOrStraightFlush(c,N) else                         \
    mFlushOrStraightFlush(d,N) else                         \
    mFlushOrStraightFlush(h,N) else                         \
	    /* total cards in other suits <= N-5; spade flush:*/ \
        if (!(i = straightValue[s]))						\
            return Value(FLUSH) | hi5RanksMask[s];			\
        else                                                \
            return (Value(STRAIGHT_FLUSH) - Value(STRAIGHT)) + i; \
    if ((i = straightValue[ranks]) != 0)					\
        return i;

#define hand0 ((int)hand & 0x1FFF)
#define hand1 (((int)hand >> 13) & 0x1FFF)
#define hand2 ((int)(hand >> 26) & 0x1FFF)
#define hand3 ((int)(hand >> 39))

d_prefix Eval_T Hand_7_Eval(Hand_T hand)
{
    Mask_T  i, j, ranks,
			c = hand0, d = hand1, h = hand2, s = hand3;

    switch (nbrOfRanks[ranks = c | d | h | s]) {

        case 2: /* quads with trips kicker */
                i = c & d & h & s;  /* bit for quads */
                return Value(FOUR_OF_A_KIND) | hiBotRank[i] with(i ^ ranks);

        case 3: /* trips and pair (full house) with non-playing pair,
                   or two trips (full house) with non-playing singleton,
                   or quads with pair and singleton */
                /* bits for singleton, if any, and trips, if any: */
                if (nbrOfRanks[i = c ^ d ^ h ^ s] == 3) {
                    /* two trips (full house) with non-playing singleton */
                    if (nbrOfRanks[i = c & d] != 2)
                        if (nbrOfRanks[i = c & h] != 2)
                            if (nbrOfRanks[i = c & s] != 2)
                                if (nbrOfRanks[i = d & h] != 2)
                                    if (nbrOfRanks[i = d & s] != 2)
                                        i = h & s;  /* bits for the trips */
                    return Value(FULL_HOUSE) | hiBotRank[i] with(i ^ hiRankMask[i]); }
                if ((j = c & d & h & s) != 0)   /* bit for quads */
                    /* quads with pair and singleton */
                    return Value(FOUR_OF_A_KIND) | hiBotRank[j] with(hiRankMask[ranks ^ j]);
                /* trips and pair (full house) with non-playing pair */
                return Value(FULL_HOUSE) | hiBotRank[i] with(hiRankMask[ranks ^ i]);

        case 4: /* three pair and singleton,
                   or trips and pair (full house) and two non-playing
                    singletons,
                   or quads with singleton kicker and two non-playing
                    singletons */
                i = c ^ d ^ h ^ s; /* the bit(s) of the trips, if any,
                                    and singleton(s) */
                if (nbrOfRanks[i] == 1) {   
                    /* three pair and singleton */
                    j = ranks ^ i;      /* the three bits for the pairs */
                    ranks = hiRankMask[j];  /* bit for the top pair */
                    j ^= ranks;         /* bits for the two bottom pairs */
                    return hiTopRankTWO_PAIR[ranks]
                        | hiBotRank[j] | hiRankMask[(hiRankMask[j] ^ j) | i]; }
                if (!(j = c & d & h & s)) {
                    /* trips and pair (full house) and two non-playing
                        singletons */
                    i ^= ranks;     /* bit for the pair */
                    if (!(j = (c & d) & (~i)))
                        j = (h & s) & (~i); /* bit for the trips */
                    return Value(FULL_HOUSE) | hiBotRank[j] with(i); }
                /* quads with singleton kicker and two
                    non-playing singletons */
                return Value(FOUR_OF_A_KIND) | hiBotRank[j] with(hiRankMask[i]);

        case 5: /* flush and/or straight,
                   or two pair and three singletons,
                   or trips and four singletons */
                mStraightAndOrFlush(7)
                i = c ^ d ^ h ^ s; /* the bits of the trips, if any,
                                      and singletons */
                if (nbrOfRanks[i] != 5) {
                    /* two pair and three singletons */
                    j = i ^ ranks;  /* the two bits for the pairs */
                    return hiTopRankTWO_PAIR[j]
                            | hiBotRank[hiRankMask[j] ^ j] | hiRankMask[i]; }
                /* trips and four singletons */
                if (!(j = c & d))
                    j = h & s;
                return Value(THREE_OF_A_KIND) | hiBotRank[j] with(hi2RanksMask[i ^ j]);

        case 6: /* flush and/or straight,
                   or one pair and three kickers and
                    two nonplaying singletons */
                mStraightAndOrFlush(7)
                i = c ^ d ^ h ^ s; /* the bits of the five singletons */
                return Value(PAIR) | hiBotRank[ranks ^ i] | hi3RanksMask[i];

        case 7: /* flush and/or straight or no pair */
                mStraightAndOrFlush(7)
                return /* Value(NO_PAIR) | */ hi5RanksMask[ranks];

        } /* end switch */

    return 0; /* never reached, but avoids compiler warning */
}

// each of the following handXlo extracts the appropriate 13-bit field from hand and
// rotates it left to position the ace in the least significant bit
#define hand0lo ( (((int)hand & 0x1FFF) << 1) + (((int)hand ^ 0x1000) >> 12) )
#define hand1lo ( (((int)hand >> 12) & 0x3FFE) + (((int)hand ^ (0x1000<<13)) >> 25) )
#define hand2lo ( ((int)(hand >> 25) & 0x3FFE) + (int)((hand ^ (0x1000i64 << 26)) >> 38) )
#define hand3lo ( ((int)(hand >> 38) & 0x3FFE) + (int)((hand ^ (0x1000i64 << 39)) >> 51) )

d_prefix Eval_T Hand_Razz_Eval(Hand_T hand)
{
    Mask_T  i, j, ranks,
			c = hand0lo,
			d = hand1lo,
			h = hand2lo,
			s = hand3lo;

	switch (nbrOfRanks[ranks = c | d | h | s]) {

        case 2: /* AAAABBB -- full house */
                i = c & d & h & s;  /* bit for quads */
				j = i ^ ranks;		/* bit for trips */
				// it can't matter in comparison of results from a 52-card deck,
				// but we return the correct value per relative ranks
				if (i > j)
					return Value(FULL_HOUSE) | hiBotRank[i] with(j);
				return Value(FULL_HOUSE) | hiBotRank[j] with(i);

        case 3: /*	AAABBBC -- two pair,
                    AAAABBC -- two pair,
 					AAABBCC -- two pair w/ kicker = highest rank.
               /* bits for singleton, if any, and trips, if any: */
                if (nbrOfRanks[i = c ^ d ^ h ^ s] == 3) {
                    /* odd number of each rank: AAABBBC -- two pair */
                    if (nbrOfRanks[i = c & d] != 2)
                        if (nbrOfRanks[i = c & h] != 2)
                            if (nbrOfRanks[i = c & s] != 2)
                                if (nbrOfRanks[i = d & h] != 2)
                                    if (nbrOfRanks[i = d & s] != 2)
                                        i = h & s;  /* bits for the trips */
                    return hiTopRankTWO_PAIR[i] |
							hiBotRank[i ^ hiRankMask[i]] | (ranks ^ i); }
                if ((j = c & d & h & s) != 0) {   /* bit for quads */
                    /* AAAABBC -- two pair */
					j = ranks ^ i;				/* bits for pairs */
                    return hiTopRankTWO_PAIR[j] |
							hiBotRank[j ^ hiRankMask[j]] | i;
                }
                /* AAABBCC -- two pair w/ kicker = highest rank */
				i = hiRankMask[ranks];			/* kicker bit */
				j = ranks ^ i;					/* pairs bits */
                return hiTopRankTWO_PAIR[j] | hiBotRank[j ^ hiRankMask[j]] | i;

        case 4: /*	AABBCCD -- one pair (lowest of A, B, C),
					AAABBCD -- one pair (A or B),
					AAAABCD -- one pair (A)
				 */
                i = c ^ d ^ h ^ s; /* the bit(s) of the trips, if any,
                                    and singleton(s) */
                if (nbrOfRanks[i] == 1) {   
                    /* AABBCCD -- one pair (C with ABD) */
					/* D's bit is in i */
					j = hi2RanksMask[ranks ^ i] | i;	/* kickers */
					return Value(PAIR) | hiBotRank[ranks ^ j] | j; }
                if (!(j = c & d & h & s)) {
                    /* AAABBCD -- one pair (A or B) */
                    i ^= ranks;     /* bit for B */
                    if (!(j = (c & d) & (~i)))
                        j = (h & s) & (~i); /* bit for A */
					if (i < j)
						return Value(PAIR) | hiBotRank[i] | (ranks ^ i);
					return Value(PAIR) | hiBotRank[j] | (ranks ^ j); }
                /* AAAABCD -- one pair (A) */
                return Value(PAIR) | hiBotRank[j] | i;

        case 5: return /* Value(NO_PAIR) | */ ranks;

        case 6: return /* Value(NO_PAIR) | */ lo5RanksMask[ranks];

        case 7:	return /* Value(NO_PAIR) | */ lo5RanksMask[ranks];

        } /* end switch */

    return 0; /* never reached, but avoids compiler warning */
}

d_prefix Eval_T Hand_6_Eval(Hand_T hand)
{
    Mask_T  i, j, ranks,
			c = hand0, d = hand1, h = hand2, s = hand3;

    switch (nbrOfRanks[ranks = c | d | h | s]) {

        case 2: /* quads with pair kicker,
				   or two trips (full house) */
				/* bits for trips, if any: */
                if (nbrOfRanks[i = c ^ d ^ h ^ s])
                    /* two trips (full house) */
					return Value(FULL_HOUSE) | hiBotRank[i]
							with(i ^ hiRankMask[i]);
				/* quads with pair kicker */
                i = c & d & h & s;  /* bit for quads */
                return Value(FOUR_OF_A_KIND) | hiBotRank[i]
							with(i ^ ranks);

		case 3:	/* quads with singleton kicker and non-playing
				    singleton,
				   or full house with non-playing singleton,
				   or two pair with non-playing pair */
				if (!(c ^ d ^ h ^ s)) {
					/* no trips or singletons:  three pair */
					i = hiRankMask[ranks];	/* bit for the top pair */
					ranks ^= i;				/* bits for the bottom two pairs */
					j = hiRankMask[ranks];	/* bit for the middle pair */
					return hiTopRankTWO_PAIR[i]
						| hiBotRank[j] | (ranks ^ j); }
				if ((i = c & d & h & s) == 0) {
					/* full house with singleton */
					if (!(i = c & d & h))
						if (!(i = c & d & s))
							if (!(i = c & h & s))
								i = d & h & s; /* bit of trips */
					j = c ^ d ^ h ^ s; /* the bits of the trips
										  and singleton */
					return Value(FULL_HOUSE) | hiBotRank[i]
							with(j ^ ranks); }
				/* quads with kicker and singleton */
				return Value(FOUR_OF_A_KIND) | hiBotRank[i]
						with(hiRankMask[i ^ ranks]);

		case 4:	/* trips and three singletons,
				   or two pair and two singletons */
				if ((i = c ^ d ^ h ^ s) != ranks) {
					/* two pair and two singletons */
                    j = i ^ ranks;  /* the two bits for the pairs */
                    return hiTopRankTWO_PAIR[j]
                            | hiBotRank[hiRankMask[j] ^ j] | hiRankMask[i]; }
				/* trips and three singletons */
				if (!(i = c & d))
					i = h & s; /* bit of trips */
				return Value(THREE_OF_A_KIND) | hiBotRank[i]
						with(hi2RanksMask[i ^ ranks]);

		case 5:	/* flush and/or straight,
				   or one pair and three kickers and
				    one non-playing singleton */
				mStraightAndOrFlush(6)
                i = c ^ d ^ h ^ s; /* the bits of the four singletons */
                return Value(PAIR) | hiBotRank[ranks ^ i] | hi3RanksMask[i];

		case 6:	/* flush and/or straight or no pair */
                mStraightAndOrFlush(6)
                return /* Value(NO_PAIR) | */ hi5RanksMask[ranks];

        } /* end switch */

    return 0; /* never reached, but avoids compiler warning */
}
#endif  /* d_asian } */

#if d_asian
d_prefix Eval_T Asian_5_Eval(Hand_T hand)
#else
d_prefix Eval_T Hand_5_Eval(Hand_T hand)
#endif
{
    Mask_T  i, j, ranks,
			c = hand0, d = hand1, h = hand2, s = hand3;

	switch (nbrOfRanks[ranks = c | d | h | s]) {

        case 2: /* quads or full house */
                i = c & d;				/* any two suits */
                if (!(i & h & s)) {     /* no bit common to all suits */
                    i = c ^ d ^ h ^ s;  /* trips bit */
                    return Value(FULL_HOUSE) | hiBotRank[i] with(i ^ ranks); }
                else
                    /* the quads bit must be present in each suit mask,
                       but the kicker bit in no more than one; so we need
                       only AND any two suit masks to get the quad bit: */
                    return Value(FOUR_OF_A_KIND) | hiBotRank[i] with(i ^ ranks);

        case 3: /* trips and two kickers,
                   or two pair and kicker */
                if ((i = c ^ d ^ h ^ s) == ranks) {
                    /* trips and two kickers */
                    if ((i = c & d) != 0)
                        return Value(THREE_OF_A_KIND) | hiBotRank[i] with(i ^ ranks);
                    if ((i = c & h) != 0)
                        return Value(THREE_OF_A_KIND) | hiBotRank[i] with(i ^ ranks);
                    i = d & h;
                    return Value(THREE_OF_A_KIND) | hiBotRank[i]
                        with(i ^ ranks); }
                /* two pair and kicker; i has kicker bit */
                j = i ^ ranks;      /* j has pairs bits */
                return hiTopRankTWO_PAIR[j] | hiBotRank[j ^ hiRankMask[j]] | i;

        case 4: /* pair and three kickers */
                i = c ^ d ^ h ^ s; /* kicker bits */
                return Value(PAIR) | hiBotRank[ranks ^ i] | i;

        case 5: /* flush and/or straight, or no pair */
				if (!(i = straightValue[ranks]))
					i = ranks;
				if (c != 0) {			/* if any clubs... */
					if (c != ranks)		/*   if no club flush... */
						return i; }		/*      return straight or no pair value */
				else
					if (d != 0) {
						if (d != ranks)
							return i; }
					else
						if (h != 0) {
							if (h != ranks)
								return i; }
					/*	else s == ranks: spade flush */
				/* There is a flush */
				if (i == ranks)
					/* no straight */
					return Value(FLUSH) | ranks;
				else
					return (Value(STRAIGHT_FLUSH) - Value(STRAIGHT)) + i;
	}

    return 0; /* never reached, but avoids compiler warning */
}

#if !d_asian

d_prefix Eval_T Hand_5_Ato5Lo_Eval(Hand_T hand)
{
    Mask_T  i, j, ranks,
			c = hand0lo,
			d = hand1lo,
			h = hand2lo,
			s = hand3lo;

	switch (nbrOfRanks[ranks = c | d | h | s]) {

        case 2: /* quads or full house */
                i = c & d;				/* any two suits */
                if (!(i & h & s)) {     /* no bit common to all suits */
                    i = c ^ d ^ h ^ s;  /* trips bit */
                    return Value(FULL_HOUSE) | hiBotRank[i] with(i ^ ranks); }
                else
                    /* the quads bit must be present in each suit mask,
                       but the kicker bit in no more than one; so we need
                       only AND any two suit masks to get the quad bit: */
                    return Value(FOUR_OF_A_KIND) | hiBotRank[i] with(i ^ ranks);

        case 3: /* trips and two kickers,
                   or two pair and kicker */
                if ((i = c ^ d ^ h ^ s) == ranks) {
                    /* trips and two kickers */
                    if ((i = c & d) != 0)
                        return Value(THREE_OF_A_KIND) | hiBotRank[i] with(i ^ ranks);
                    if ((i = c & h) != 0)
                        return Value(THREE_OF_A_KIND) | hiBotRank[i] with(i ^ ranks);
                    i = d & h;
                    return Value(THREE_OF_A_KIND) | hiBotRank[i]
                        with(i ^ ranks); }
                /* two pair and kicker; i has kicker bit */
                j = i ^ ranks;      /* j has pairs bits */
                return hiTopRankTWO_PAIR[j] | hiBotRank[j ^ hiRankMask[j]] | i;

        case 4: /* pair and three kickers */
                i = c ^ d ^ h ^ s; /* kicker bits */
                return Value(PAIR) | hiBotRank[ranks ^ i] | i;

        case 5: /* no pair */
				return ranks;
	}

    return 0; /* never reached, but avoids compiler warning */
}
#endif

d_prefix int Hi_Card_Mask(short mask)
{
    return hiRankMask[mask];
}

d_prefix int Number_Of_Ranks(short mask)
{
    return nbrOfRanks[mask];
}

d_prefix int Rank_Of_Hi_Card(short mask)
{
    return hiBotRank[mask] >> kBotRankShift;
}

d_prefix void Decode(Hand_T hand, int *rank, int *suit)
{
    int m;

    for (*suit = 0; *suit < 4; ++*suit)
        if ((m = (int)(hand >> (*suit*13)) & 0x1FFF) != 0)
            for (*rank = 2; *rank <= 14; ++*rank, m >>= 1)
                if (m & 1)
                    return;
}

#if !d_asian
d_prefix Eval_T Hand_8Low_Eval(Hand_T hand) {

	return loEvalOrNo8Low[hand0lo | hand1lo | hand2lo | hand3lo];
}

d_prefix Eval_T Ranks_8Low_Eval(int ranks) {

	return loEvalOrNo8Low[ranks];
}

d_prefix Eval_T Omaha8_Low_Eval(int twoHoleRanks, int boardRanks) {
	return loEvalOrNo8Low[lo3RanksMask[boardRanks & ~twoHoleRanks] | twoHoleRanks];
}

d_prefix Eval_T No8LowValue(void) {
	return k_No8Low;
}
#endif

/************ Initialization ***********************/

static void SetStraight(int ts)
/* must call with ts from A..T to 5..A in that order */
{
    int es, i, j;

    for (i = 0x1000; i > 0; i >>= 1)
        for (j = 0x1000; j > 0; j >>= 1) {
            es = ts | i | j;	/* 5 straight bits plus up to two other bits */
			if (!straightValue[es])
				if (ts == d_wheel)
#if d_asian
                    straightValue[es] = Value(STRAIGHT) + (10 << kBotRankShift);
#else
					straightValue[es] = Value(STRAIGHT) + (5 << kBotRankShift); 
#endif
				else
					straightValue[es] = Value(STRAIGHT) + hiBotRank[ts];
		}
}

#if d_asian
d_prefix Boolean Init_Asian_Eval(void)
#else
d_prefix Boolean Init_Hand_Eval(void)
#endif
{
  static	Boolean initted = false;

    int     mask, bitCount;
    Mask_T  shiftReg, i;
	Mask_T	value;

	if (initted)
		return true;

#define mAllocate(type, array)                                  \
    array = (type *)(d_ram_alloc(kArraySize, sizeof(*array)));  \
    if (!array) return false

    mAllocate(Mask_T, straightValue);
    mAllocate(int, nbrOfRanks);
    mAllocate(Mask_T, hiTopRankTWO_PAIR);
    mAllocate(Mask_T, hiBotRank);
    mAllocate(Mask_T, hiRankMask);
    mAllocate(Mask_T, hi2RanksMask);
    mAllocate(Mask_T, hi3RanksMask);
    mAllocate(Mask_T, hi5RanksMask);
    mAllocate(Mask_T, lo5RanksMask);
	mAllocate(Mask_T, lo3RanksMask);
	mAllocate(Eval_T, loEvalOrNo8Low);

    for (mask = 1; mask < kArraySize; ++mask) {
        bitCount = 0;
        shiftReg = mask;
        for (i = kAce - 1; i > 0; --i, shiftReg <<= 1)
            if (shiftReg & 0x1000)
                switch (++bitCount) {
                    case 1: hiTopRankTWO_PAIR[mask] = Value(TWO_PAIR) 
                                                | ((i + 1) << kTopRankShift);
							hiBotRank[mask] = (i + 1) << kBotRankShift;
                            hiRankMask[mask] = 0x1000 >> (kAce - 1 - i);
                            break;
                    case 2: hi2RanksMask[mask] = (shiftReg & 0x03FFF000)
                                                    >> (kAce - 1 - i);
                            break;
                    case 3: hi3RanksMask[mask] = (shiftReg & 0x03FFF000)
                                                    >> (kAce - 1 - i);
                            break;
                    case 5: hi5RanksMask[mask] = (shiftReg & 0x03FFF000)
                                                    >> (kAce - 1 - i);
				}
        nbrOfRanks[mask] = bitCount;

        bitCount = 0;
        /* rotate the 13 bits left to get ace into LS bit */
        /* we don't need to mask the low 13 bits of the result */
        /* as we're going to look only at the low order 8 bits */
        shiftReg = (mask << 1) + ((mask ^ 0x1000) >> 12);
        value = 0;
        for (i = 0; i < 8; ++i, shiftReg >>= 1)
	        if ((shiftReg & 1) != 0) {
		        value |= (1 << i); /* undo previous shifts, copy bit */
		        if (++bitCount == 5) {
			        lo5RanksMask[mask] = value;
			        break;
		        }
		        if (bitCount == 3)
			        lo3RanksMask[mask] = value;
	        }
        loEvalOrNo8Low[mask] = (bitCount == 5) ? value : k_No8Low;
		  }

    for (mask = 0x1F00/*A..T*/; mask >= 0x001F/*6..2*/; mask >>= 1)
        SetStraight(mask);
    SetStraight(d_wheel);       /* A,5..2 (A,T..7 for Asian stud) */

	initted = true;

  return true;
}
