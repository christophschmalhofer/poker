/* $Id: pokutil.h,v 1.2 2002/05/18 22:39:43 mjmaurer Exp $ */

#ifndef POKUTIL_H
#define POKUTIL_H
#include <jni.h>	/* for JNI types */
#include "poker_defs.h"	/* for *_CardMask types */
#include "deck_std.h"
#include "deck_joker.h"
#include "deck_astud.h"

extern int numCardsStandard(StdDeck_CardMask mask);
extern int numCardsJoker(JokerDeck_CardMask mask);
extern int numCardsAsianStud(AStudDeck_CardMask mask);

extern StdDeck_CardMask parseLongStandard(jlong lmask);

extern int
parseStandardRanksSuits(JNIEnv *env, jintArray ranks, jintArray suits,
                        StdDeck_CardMask *mcards, int *ncards);

extern int
parseJokerRanksSuits(JNIEnv *env, jintArray ranks, jintArray suits,
                     JokerDeck_CardMask *mcards, int *ncards);

extern int
parseAsianStudRanksSuits(JNIEnv *env, jintArray ranks, jintArray suits,
                         AStudDeck_CardMask *mcards, int *ncards);

#endif
