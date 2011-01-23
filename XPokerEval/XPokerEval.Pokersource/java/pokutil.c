/* $Id: pokutil.c,v 1.2 2002/05/18 22:39:43 mjmaurer Exp $ */

#include <jni.h>
#include "jniutil.h"		/* JNI helpers: throw exceptions */
#include "poker_defs.h"		/* poker-eval basic definitions */
#include "pokutil.h"

/******************************************************************************
  Standard deck
******************************************************************************/
#include "deck_std.h"
#include "rules_std.h"

int
numCardsStandard(StdDeck_CardMask mask)
{
  int r, s;
  int nbits = 0;
  for (r=StdDeck_Rank_FIRST; r<=StdDeck_Rank_LAST; r++) {
    for (s=StdDeck_Suit_FIRST; s<=StdDeck_Suit_LAST; s++) {
      StdDeck_CardMask card = StdDeck_MASK(StdDeck_MAKE_CARD(r, s));
      if (StdDeck_CardMask_ANY_SET(mask, card))
        nbits++;
    }
  }
  return nbits;
}

StdDeck_CardMask
parseLongStandard(jlong lmask)
{
  int i;
  jlong flag;
  StdDeck_CardMask mask;

  StdDeck_CardMask_RESET(mask);
  for (i=0, flag=1; i<StdDeck_N_CARDS; i++, flag<<=1)
    if (lmask & flag)
      StdDeck_CardMask_SET(mask, i);
  return mask;
}      

int
parseStandardRanksSuits(JNIEnv *env, jintArray ranks, jintArray suits,
                        StdDeck_CardMask *mcards, int *ncards)
{
  int i, status = 0;
  jsize nranks = (*env)->GetArrayLength(env, ranks);
  jsize nsuits = (*env)->GetArrayLength(env, suits);
  jint *jranks = (*env)->GetIntArrayElements(env, ranks, 0);
  jint *jsuits = (*env)->GetIntArrayElements(env, suits, 0);

  StdDeck_CardMask_RESET(*mcards);
  *ncards = 0;
  if (nranks != nsuits) {
    status = 1;
    goto release1;
  }
  for (i=0; i<nranks; i++) {
    int card = StdDeck_MAKE_CARD(jranks[i], jsuits[i]);
    if (StdDeck_CardMask_CARD_IS_SET(*mcards, card)) {
      status = 1;
      goto release1;
    }
    StdDeck_CardMask_SET(*mcards, card);
    (*ncards)++;
  }
#ifdef DEBUG
  printf("In C: Hand %s\n", DmaskString(StdDeck, *mcards));
#endif

 release1:
  (*env)->ReleaseIntArrayElements(env, ranks, jranks, JNI_ABORT);
  (*env)->ReleaseIntArrayElements(env, suits, jsuits, JNI_ABORT);
  return status;
}

/******************************************************************************
  Joker deck
******************************************************************************/
#include "deck_joker.h"
#include "rules_joker.h"

int
parseJokerRanksSuits(JNIEnv *env, jintArray ranks, jintArray suits,
                     JokerDeck_CardMask *mcards, int *ncards)
{
  int i, status = 0;
  jsize nranks = (*env)->GetArrayLength(env, ranks);
  jsize nsuits = (*env)->GetArrayLength(env, suits);
  jint *jranks = (*env)->GetIntArrayElements(env, ranks, 0);
  jint *jsuits = (*env)->GetIntArrayElements(env, suits, 0);

  JokerDeck_CardMask_RESET(*mcards);
  *ncards = 0;
  if (nranks != nsuits) {
    status = 1;
    goto release2;
  }
  for (i=0; i<nranks; i++) {
    int card;
    if (jranks[i] == JokerDeck_Rank_LAST + 1) {
      /* encode joker as any card with rank one higher than highest; this
         corresponds to rank 13 */
      card = JokerDeck_JOKER;
    } else {
      card = JokerDeck_MAKE_CARD(jranks[i], jsuits[i]);
    }
    if (JokerDeck_CardMask_CARD_IS_SET(*mcards, card)) {
      status = 1;
      goto release2;
    }
    JokerDeck_CardMask_SET(*mcards, card);
    (*ncards)++;
  }
#ifdef DEBUG
  printf("In C: Hand %s\n", DmaskString(JokerDeck, *mcards));
#endif

 release2:
  (*env)->ReleaseIntArrayElements(env, ranks, jranks, JNI_ABORT);
  (*env)->ReleaseIntArrayElements(env, suits, jsuits, JNI_ABORT);
  return status;
}

int
numCardsJoker(JokerDeck_CardMask mask)
{
  int r, s;
  int nbits = 0;
  JokerDeck_CardMask card;
  for (r=JokerDeck_Rank_FIRST; r<=JokerDeck_Rank_LAST; r++) {
    for (s=JokerDeck_Suit_FIRST; s<=JokerDeck_Suit_LAST; s++) {
      card = JokerDeck_MASK(JokerDeck_MAKE_CARD(r, s));
      if (JokerDeck_CardMask_ANY_SET(mask, card))
        nbits++;
    }
  }
  card = JokerDeck_MASK(JokerDeck_JOKER);
  if (JokerDeck_CardMask_ANY_SET(mask, card))
    nbits++;
  return nbits;
}


/******************************************************************************
  Asian Stud deck
******************************************************************************/
#include "deck_astud.h"
#include "rules_astud.h"

int
parseAsianStudRanksSuits(JNIEnv *env, jintArray ranks, jintArray suits,
                         AStudDeck_CardMask *mcards, int *ncards)
{
  int i, status = 0;
  jsize nranks = (*env)->GetArrayLength(env, ranks);
  jsize nsuits = (*env)->GetArrayLength(env, suits);
  jint *jranks = (*env)->GetIntArrayElements(env, ranks, 0);
  jint *jsuits = (*env)->GetIntArrayElements(env, suits, 0);

  AStudDeck_CardMask_RESET(*mcards);
  *ncards = 0;
  if (nranks != nsuits) {
    status = 1;
    goto release3;
  }
  for (i=0; i<nranks; i++) {
    int card = AStudDeck_MAKE_CARD(jranks[i], jsuits[i]);
    if (AStudDeck_CardMask_CARD_IS_SET(*mcards, card)) {
      status = 1;
      goto release3;
    }
    AStudDeck_CardMask_SET(*mcards, card);
    (*ncards)++;
  }
#ifdef DEBUG
  printf("In C: Hand %s\n", DmaskString(AStudDeck, *mcards));
#endif

 release3:
  (*env)->ReleaseIntArrayElements(env, ranks, jranks, JNI_ABORT);
  (*env)->ReleaseIntArrayElements(env, suits, jsuits, JNI_ABORT);
  return status;
}

int
numCardsAsianStud(AStudDeck_CardMask mask)
{
  int r, s;
  int nbits = 0;
  for (r=AStudDeck_Rank_FIRST; r<=AStudDeck_Rank_LAST; r++) {
    for (s=AStudDeck_Suit_FIRST; s<=AStudDeck_Suit_LAST; s++) {
      AStudDeck_CardMask card = AStudDeck_MASK(AStudDeck_MAKE_CARD(r, s));
      if (AStudDeck_CardMask_ANY_SET(mask, card))
        nbits++;
    }
  }
  return nbits;
}

