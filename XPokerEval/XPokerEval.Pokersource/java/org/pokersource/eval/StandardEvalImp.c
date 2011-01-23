/* $Id: StandardEvalImp.c,v 1.2 2002/05/02 22:24:33 mjmaurer Exp $ */

#include <stdio.h>
#include <jni.h>
#include "pokerjni.h"	/* javah output for us to implement */
#include "jniutil.h"	/* JNI help like exception throwing */
#include "poker_defs.h"	/* poker-eval basics */
#include "pokutil.h"	/* poker-eval help like card parsing */

#include "deck_std.h"
#include "rules_std.h"
#include "inlines/eval.h"
#include "inlines/eval_low.h"
#include "inlines/eval_low8.h"

JNIEXPORT jlong JNICALL Java_org_pokersource_eval_StandardEval_EvalHigh
   (JNIEnv *env, jclass class, jintArray ranks, jintArray suits)
{
  int ncards;
  StdDeck_CardMask mcards;
  HandVal hival;
  
  if (parseStandardRanksSuits(env, ranks, suits, &mcards, &ncards)) {
    jniThrow(env, class, "unable to parse input cards");
    return (jlong)0;
  }
  hival = StdDeck_StdRules_EVAL_N(mcards, ncards);
#ifdef DEBUG
  printf("In C: Hand [%s] => Eval: %d ",
         DmaskString(StdDeck, mcards), hival);
  StdRules_HandVal_print(hival);
  printf("\n");
#endif
  return (jlong)hival;
}

JNIEXPORT jlong JNICALL Java_org_pokersource_eval_StandardEval_EvalLow
   (JNIEnv *env, jclass class, jintArray ranks, jintArray suits)
{
  int ncards;
  StdDeck_CardMask mcards;
  LowHandVal loval;
  
  if (parseStandardRanksSuits(env, ranks, suits, &mcards, &ncards)) {
    jniThrow(env, class, "unable to parse input cards");
    return (jlong)0;
  }
  loval = StdDeck_Lowball_EVAL(mcards, ncards);
#ifdef DEBUG
  printf("In C: Hand [%s] => Eval: %d ",
         DmaskString(StdDeck, mcards), loval);
  LowHandVal_print(loval);
  printf("\n");
#endif
  return (jlong)loval;
}

JNIEXPORT jlong JNICALL Java_org_pokersource_eval_StandardEval_EvalLow8
   (JNIEnv *env, jclass class, jintArray ranks, jintArray suits)
{
  int ncards;
  StdDeck_CardMask mcards;
  LowHandVal lo8val;
  
  if (parseStandardRanksSuits(env, ranks, suits, &mcards, &ncards)) {
    jniThrow(env, class, "unable to parse input cards");
    return (jlong)0;
  }
  lo8val = StdDeck_Lowball8_EVAL(mcards, ncards);
#ifdef DEBUG
  printf("In C: Hand [%s] => Eval: %d ",
         DmaskString(StdDeck, mcards), lo8val);
  LowHandVal_print(lo8val);
  printf("\n");
#endif
  return (jlong)lo8val;
}

