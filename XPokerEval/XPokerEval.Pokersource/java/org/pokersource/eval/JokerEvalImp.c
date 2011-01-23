/* $Id: JokerEvalImp.c,v 1.2 2002/05/02 22:24:33 mjmaurer Exp $ */

#include <stdio.h>
#include <jni.h>
#include "pokerjni.h"	/* javah output for us to implement */
#include "jniutil.h"	/* JNI help like exception throwing */
#include "poker_defs.h"	/* poker-eval basics */
#include "pokutil.h"	/* poker-eval help like card parsing */

#include "deck_joker.h"
#include "rules_joker.h"
#include "inlines/eval_joker.h"
#include "inlines/eval_joker_low.h"
#include "inlines/eval_joker_low8.h"

JNIEXPORT jlong JNICALL Java_org_pokersource_eval_JokerEval_EvalHigh
   (JNIEnv *env, jclass class, jintArray ranks, jintArray suits)
{
  int ncards;
  JokerDeck_CardMask mcards;
  HandVal hival;
  
  if (parseJokerRanksSuits(env, ranks, suits, &mcards, &ncards)) {
    jniThrow(env, class, "unable to parse input cards");
    return (jlong)0;
  }
  hival = JokerDeck_JokerRules_EVAL_N(mcards, ncards);
#ifdef DEBUG
  printf("In C: Hand [%s] => Eval: %d ",
         DmaskString(JokerDeck, mcards), hival);
  JokerRules_HandVal_print(hival);
  printf("\n");
#endif
  return (jlong)hival;
}

JNIEXPORT jlong JNICALL Java_org_pokersource_eval_JokerEval_EvalLow
   (JNIEnv *env, jclass class, jintArray ranks, jintArray suits)
{
  int ncards;
  JokerDeck_CardMask mcards;
  LowHandVal loval;
  
  if (parseJokerRanksSuits(env, ranks, suits, &mcards, &ncards)) {
    jniThrow(env, class, "unable to parse input cards");
    return (jlong)0;
  }
  loval = JokerDeck_Lowball_EVAL(mcards, ncards);
#ifdef DEBUG
  printf("In C: Hand [%s] => Eval: %d ",
         DmaskString(JokerDeck, mcards), loval);
  LowHandVal_print(loval);
  printf("\n");
#endif
  return (jlong)loval;
}

JNIEXPORT jlong JNICALL Java_org_pokersource_eval_JokerEval_EvalLow8
   (JNIEnv *env, jclass class, jintArray ranks, jintArray suits)
{
  int ncards;
  JokerDeck_CardMask mcards;
  LowHandVal lo8val;
  
  if (parseJokerRanksSuits(env, ranks, suits, &mcards, &ncards)) {
    jniThrow(env, class, "unable to parse input cards");
    return (jlong)0;
  }
  lo8val = JokerDeck_Lowball8_EVAL(mcards, ncards);
#ifdef DEBUG
  printf("In C: Hand [%s] => Eval: %d ",
         DmaskString(JokerDeck, mcards), lo8val);
  LowHandVal_print(lo8val);
  printf("\n");
#endif
  return (jlong)lo8val;
}

