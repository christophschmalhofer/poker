/* $Id: EnumerateImp.c,v 1.2 2002/05/18 22:39:43 mjmaurer Exp $ */

#undef DEBUG

#include <stdio.h>
#include <jni.h>
#include "pokerjni.h"	/* javah output for us to implement */
#include "jniutil.h"	/* JNI help like exception throwing */
#include "poker_defs.h"	/* poker-eval basics */
#include "pokutil.h"	/* poker-eval help like card parsing */

#include "poker_defs.h"
#include "enumdefs.h"

JNIEXPORT void JNICALL 
Java_org_pokersource_enum_Enumerate_PotEquity__II_3_3I_3_3I_3I_3I_3I_3I_3D
  (JNIEnv *env , jclass class, jint gameType, jint nsamples,
   jobjectArray pocketRanks, jobjectArray pocketSuits,
   jintArray boardRanks, jintArray boardSuits,
   jintArray deadRanks, jintArray deadSuits,
   jdoubleArray ev)
{
  int i, nplayers, npockets[ENUM_MAXPLAYERS], nboard, ndead, err;
  StdDeck_CardMask pockets[ENUM_MAXPLAYERS], board, dead;
  enum_result_t result;

  nplayers = (*env)->GetArrayLength(env, pocketRanks);
  if (nplayers > ENUM_MAXPLAYERS) {
    jniThrow(env, class, "too many players");
    return;
  }

  /* parse list of dead cards */
  if (parseStandardRanksSuits(env, deadRanks, deadSuits, &dead, &ndead)) {
    jniThrow(env, class, "unable to parse input cards");
    return;
  }

  /* parse list of pocket cards for each player, also add to dead cards */
  for (i=0; i<nplayers; i++) {
     jintArray ranks = (jintArray)
       (*env)->GetObjectArrayElement(env, pocketRanks, i);
     jintArray suits = (jintArray)
       (*env)->GetObjectArrayElement(env, pocketSuits, i);
     if (parseStandardRanksSuits(env, ranks, suits,
                                 &pockets[i], &npockets[i])) {
       jniThrow(env, class, "unable to parse input cards");
       return;
     }
     StdDeck_CardMask_OR(dead, dead, pockets[i]);
  }

  /* parse list of board cards, also add to dead cards */
  if (parseStandardRanksSuits(env, boardRanks, boardSuits, &board, &nboard)) {
    jniThrow(env, class, "unable to parse input cards");
    return;
  }
  StdDeck_CardMask_OR(dead, dead, board);

  /* do the heavy lifting of enumeration */
  if (nsamples == 0) {
    err = enumExhaustive(gameType, pockets, board, dead, nplayers, nboard,
                         &result);
  } else {
    err = enumSample(gameType, pockets, board, dead, nplayers, nboard, nsamples,
                     &result);
  }
#ifdef DEBUG
  enumResultPrint(&result, pockets, board);
#endif
  /* scale EV result and send back to Java side */
  for (i=0; i<nplayers; i++)
    result.ev[i] /= result.nsamples;
  (*env)->SetDoubleArrayRegion(env, ev, 0, nplayers, (jdouble *)result.ev);
  return;
}


JNIEXPORT void JNICALL 
Java_org_pokersource_enum_Enumerate_PotEquity__II_3JJJ_3D
  (JNIEnv *env, jclass class, jint gameType, jint nsamples,
   jlongArray lpockets, jlong jboard, jlong jdead, jdoubleArray ev)
{
  int i, nplayers, nboard, err;
  jlong *jpockets;
  StdDeck_CardMask board, dead, pockets[ENUM_MAXPLAYERS];
  enum_result_t result;

  //printf("In C: jboard=%lld jdead=%lld\n", jboard, jdead);
  nplayers = (*env)->GetArrayLength(env, lpockets);
  if (nplayers > ENUM_MAXPLAYERS) {
    jniThrow(env, class, "too many players");
    return;
  }

  /* parse list of dead cards */
  dead = parseLongStandard(jdead);
  //printf("In C: dead=%s\n", DmaskString(StdDeck, dead));

  /* parse list of pocket cards for each player, also add to dead cards */
  jpockets = (*env)->GetLongArrayElements(env, lpockets, 0);
  for (i=0; i<nplayers; i++) {
    pockets[i] = parseLongStandard(jpockets[i]);
    //printf("In C: jpockets[%d]=%lld\n", i, jpockets[i]);
    //printf("In C: pockets[%d]=%s\n", i, DmaskString(StdDeck, pockets[i]));
    StdDeck_CardMask_OR(dead, dead, pockets[i]);
  }
  (*env)->ReleaseLongArrayElements(env, lpockets, jpockets, JNI_ABORT);
  //printf("In C: dead=%s\n", DmaskString(StdDeck, dead));

  /* parse list of board cards, also add to dead cards */
  board = parseLongStandard(jboard);
  nboard = numCardsStandard(board);
  StdDeck_CardMask_OR(dead, dead, board);
  //printf("In C: nboard=%d board=%s\n", nboard, DmaskString(StdDeck, board));
  //printf("In C: dead=%s\n", DmaskString(StdDeck, dead));

  /* do the heavy lifting of enumeration */
  if (nsamples == 0) {
    err = enumExhaustive(gameType, pockets, board, dead, nplayers, nboard,
                         &result);
  } else {
    err = enumSample(gameType, pockets, board, dead, nplayers, nboard, nsamples,
                     &result);
  }
#ifdef DEBUG
  enumResultPrint(&result, pockets, board);
  fflush(stdout);
#endif
  /* scale EV result and send back to Java side */
  for (i=0; i<nplayers; i++)
    result.ev[i] /= result.nsamples;
  (*env)->SetDoubleArrayRegion(env, ev, 0, nplayers, (jdouble *)result.ev);
  return;
}

