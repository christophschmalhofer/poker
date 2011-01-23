#include "PSim_PSim.h"
#include "../../psim.hpp"

#include <iostream>


///Will set a SimResults struct into a SimResults java class.
void SetResultsSR(const SimResults& results, jobject javaResults, JNIEnv* env)
{
    //Grab the class.
    jclass cls = env->GetObjectClass(javaResults);

    //From the class, get all the fields ready to be set.
    jfieldID win = env->GetFieldID(cls, "win", "F");
    jfieldID tie = env->GetFieldID(cls, "tie", "F");
    jfieldID lose = env->GetFieldID(cls, "lose", "F");

    jfieldID winSd = env->GetFieldID(cls, "winSd", "F");
    jfieldID tieSd = env->GetFieldID(cls, "tieSd", "F");
    jfieldID loseSd = env->GetFieldID(cls, "loseSd", "F");

    jfieldID dNow = env->GetFieldID(cls, "dNow", "F");
    jfieldID d94 = env->GetFieldID(cls, "d94", "F");
    jfieldID d90 = env->GetFieldID(cls, "d90", "F");

    #ifdef CountEvaluations
        jfieldID evaluations = env->GetFieldID(cls, "evaluations", "J"); //Yeah, a long uses J for it's code. Makes sense. ;-)
    #endif

    env->SetFloatField(javaResults, win, results.win);
    env->SetFloatField(javaResults, tie, results.tie);
    env->SetFloatField(javaResults, lose, results.lose);

    env->SetFloatField(javaResults, winSd, results.winSd);
    env->SetFloatField(javaResults, tieSd, results.tieSd);
    env->SetFloatField(javaResults, loseSd, results.loseSd);

    env->SetFloatField(javaResults, dNow, results.dNow);
    env->SetFloatField(javaResults, d94, results.d94);
    env->SetFloatField(javaResults, d90, results.d90);

    #ifdef CountEvaluations
        env->SetLongField(javaResults, evaluations, results.evaluations);
    #endif
}


///Will set a PostFlopState struct into a PostFlopState java class.
void ResultsPFS(PostFlopState& results, jobject javaResults, JNIEnv* env, bool toJava = true)
{
    //Grab the class.
    jclass cls = env->GetObjectClass(javaResults);

    jfieldID UsesBoth = env->GetFieldID(cls, "UsesBoth", "Z");
    jfieldID UsesOne = env->GetFieldID(cls, "UsesOne", "Z");
    jfieldID UsesNone = env->GetFieldID(cls, "UsesNone", "Z");

    jfieldID IsNoPair = env->GetFieldID(cls, "IsNoPair", "Z");
    jfieldID IsOnePair = env->GetFieldID(cls, "IsOnePair", "Z");
    jfieldID IsTwoPair = env->GetFieldID(cls, "IsTwoPair", "Z");
    jfieldID IsTrips = env->GetFieldID(cls, "IsTrips", "Z");
    jfieldID IsStraight = env->GetFieldID(cls, "IsStraight", "Z");
    jfieldID IsFlush = env->GetFieldID(cls, "IsFlush", "Z");
    jfieldID IsFullHouse = env->GetFieldID(cls, "IsFullHouse", "Z");
    jfieldID IsQuads = env->GetFieldID(cls, "IsQuads", "Z");
    jfieldID IsStFlush = env->GetFieldID(cls, "IsStFlush", "Z");

    jfieldID IsTopPair = env->GetFieldID(cls, "IsTopPair", "Z");
    jfieldID IsMiddlePair = env->GetFieldID(cls, "IsMiddlePair", "Z");
    jfieldID IsBottomPair = env->GetFieldID(cls, "IsBottomPair", "Z");
    jfieldID IsOverPair = env->GetFieldID(cls, "IsOverPair", "Z");
    jfieldID IsOverCards = env->GetFieldID(cls, "IsOverCards", "Z");

    jfieldID IsStraightDrawPossible = env->GetFieldID(cls, "IsStraightDrawPossible", "Z");
    jfieldID IsStraightPossible = env->GetFieldID(cls, "IsStraightPossible", "Z");

    jfieldID IsFlushDrawPossible = env->GetFieldID(cls, "IsFlushDrawPossible", "Z");
    jfieldID IsFlushPossible = env->GetFieldID(cls, "IsFlushPossible", "Z");

    jfieldID IsFullHousePossible = env->GetFieldID(cls, "IsFullHousePossible", "Z");

    jfieldID Is3Flush = env->GetFieldID(cls, "Is3Flush", "Z");
    jfieldID Is4Flush = env->GetFieldID(cls, "Is4Flush", "Z");

    jfieldID FlushOuts = env->GetFieldID(cls, "FlushOuts", "I");
    jfieldID StraightOuts = env->GetFieldID(cls, "StraightOuts", "I");
    jfieldID BetterOuts = env->GetFieldID(cls, "BetterOuts", "I");

    if (toJava)
    {
        env->SetBooleanField(javaResults, UsesBoth, results.UsesBoth);
        env->SetBooleanField(javaResults, UsesOne, results.UsesOne);
        env->SetBooleanField(javaResults, UsesNone, results.UsesNone);

        env->SetBooleanField(javaResults, IsNoPair, results.IsNoPair);
        env->SetBooleanField(javaResults, IsOnePair, results.IsOnePair);
        env->SetBooleanField(javaResults, IsTwoPair, results.IsTwoPair);
        env->SetBooleanField(javaResults, IsTrips, results.IsTrips);
        env->SetBooleanField(javaResults, IsStraight, results.IsStraight);
        env->SetBooleanField(javaResults, IsFlush, results.IsFlush);
        env->SetBooleanField(javaResults, IsFullHouse, results.IsFullHouse);
        env->SetBooleanField(javaResults, IsQuads, results.IsQuads);
        env->SetBooleanField(javaResults, IsStFlush, results.IsStFlush);

        env->SetBooleanField(javaResults, IsTopPair, results.IsTopPair);
        env->SetBooleanField(javaResults, IsMiddlePair, results.IsMiddlePair);
        env->SetBooleanField(javaResults, IsBottomPair, results.IsBottomPair);
        env->SetBooleanField(javaResults, IsOverPair, results.IsOverPair);
        env->SetBooleanField(javaResults, IsOverCards, results.IsOverCards);

        env->SetBooleanField(javaResults, IsStraightDrawPossible, results.IsStraightDrawPossible);
        env->SetBooleanField(javaResults, IsStraightPossible, results.IsStraightPossible);

        env->SetBooleanField(javaResults, IsFlushDrawPossible, results.IsFlushDrawPossible);
        env->SetBooleanField(javaResults, IsFlushPossible, results.IsFlushPossible);

        env->SetBooleanField(javaResults, IsFullHousePossible, results.IsFullHousePossible);

        env->SetBooleanField(javaResults, Is3Flush, results.Is3Flush);
        env->SetBooleanField(javaResults, Is4Flush, results.Is4Flush);

        env->SetIntField(javaResults, FlushOuts, results.FlushOuts);
        env->SetIntField(javaResults, StraightOuts, results.StraightOuts);
        env->SetIntField(javaResults, BetterOuts, results.BetterOuts);
    }
    else
    {
        results.UsesBoth = env->GetBooleanField(javaResults, UsesBoth);
        results.UsesOne = env->GetBooleanField(javaResults, UsesOne);
        results.UsesNone = env->GetBooleanField(javaResults, UsesNone);

        results.IsNoPair = env->GetBooleanField(javaResults, IsNoPair);
        results.IsOnePair = env->GetBooleanField(javaResults, IsOnePair);
        results.IsTwoPair = env->GetBooleanField(javaResults, IsTwoPair);
        results.IsTrips = env->GetBooleanField(javaResults, IsTrips);
        results.IsStraight = env->GetBooleanField(javaResults, IsStraight);
        results.IsFlush = env->GetBooleanField(javaResults, IsFlush);
        results.IsFullHouse = env->GetBooleanField(javaResults, IsFullHouse);
        results.IsQuads = env->GetBooleanField(javaResults, IsQuads);
        results.IsStFlush = env->GetBooleanField(javaResults, IsStFlush);

        results.IsTopPair = env->GetBooleanField(javaResults, IsTopPair);
        results.IsMiddlePair = env->GetBooleanField(javaResults, IsMiddlePair);
        results.IsBottomPair = env->GetBooleanField(javaResults, IsBottomPair);
        results.IsOverPair = env->GetBooleanField(javaResults, IsOverPair);
        results.IsOverCards = env->GetBooleanField(javaResults, IsOverCards);

        results.IsStraightDrawPossible = env->GetBooleanField(javaResults, IsStraightDrawPossible);
        results.IsStraightPossible = env->GetBooleanField(javaResults, IsStraightPossible);

        results.IsFlushDrawPossible = env->GetBooleanField(javaResults, IsFlushDrawPossible);
        results.IsFlushPossible = env->GetBooleanField(javaResults, IsFlushPossible);

        results.IsFullHousePossible = env->GetBooleanField(javaResults, IsFullHousePossible);

        results.Is3Flush = env->GetBooleanField(javaResults, Is3Flush);
        results.Is4Flush = env->GetBooleanField(javaResults, Is4Flush);

        results.FlushOuts = env->GetIntField(javaResults, FlushOuts);
        results.StraightOuts = env->GetIntField(javaResults, StraightOuts);
        results.BetterOuts = env->GetIntField(javaResults, BetterOuts);
    }
}


/*
 * Class:     PSim_PSim
 * Method:    GetHandState
 * Signature: (Ljava/lang/String;LPSim/PostFlopState;)V
 */
JNIEXPORT void JNICALL Java_PSim_PSim_GetHandState__Ljava_lang_String_2LPSim_PostFlopState_2 (JNIEnv* env, jclass cls, jstring hand, jobject results)
{
    //Convert the string to ascii.
    const char* str;
    str = env->GetStringUTFChars(hand, false);

    PostFlopState p;
    GetHandState(str, &p);

    //Set the results back into java.
    ResultsPFS(p, results, env);

    //Don't forget to release the string.
    env->ReleaseStringUTFChars(hand, str);
}


/*
 * Class:     PSim
 * Method:    RankHand
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_PSim_PSim_RankHand__Ljava_lang_String_2 (JNIEnv* env, jclass cls, jstring hand)
{
    //Note that we are converting an unsigned int to a signed int here.
    //But, AFAIK, poker eval won't overflow it.

    //Convert the string to ascii.
    const char* str;
    str = env->GetStringUTFChars(hand, false);

    //Grab the hand's rank.
    jint result = RankHand(str);

    //Don't forget to release the string.
    env->ReleaseStringUTFChars(hand, str);

    return result;
}


/*
 * Class:     PSim
 * Method:    SimulateHand
 * Signature: (Ljava/lang/String;LSimResults;FFI)V
 */
JNIEXPORT void JNICALL Java_PSim_PSim_SimulateHand__Ljava_lang_String_2LPSim_SimResults_2FFI (JNIEnv* env, jclass cls, jstring hand, jobject results, jfloat a, jfloat b, jint c)
{
    //Convert the string to ascii.
    const char* str;
    str = env->GetStringUTFChars(hand, false);

    //Run the simulation.
    SimResults r;
    SimulateHand(str, &r, a, b, c);

    //Set the results back into java.
    SetResultsSR(r, results, env);

    //Don't forget to release the string.
    env->ReleaseStringUTFChars(hand, str);
}


/*
 * Class:     PSim
 * Method:    SimulateHandMulti
 * Signature: (Ljava/lang/String;LSimResults;III)V
 */
JNIEXPORT void JNICALL Java_PSim_PSim_SimulateHandMulti__Ljava_lang_String_2LPSim_SimResults_2III (JNIEnv* env, jclass cls, jstring hand, jobject results, jint a, jint b, jint c)
{
    //Convert the string to ascii.
    const char* str;
    str = env->GetStringUTFChars(hand, false);

    //Run the simulation.
    SimResults r;
    SimulateHandMulti(str, &r, a, b, c);

    //Set the results back into java.
    SetResultsSR(r, results, env);

    //Don't forget to release the string.
    env->ReleaseStringUTFChars(hand, str);
}


/*
 * Class:     PSim_PSim
 * Method:    GetHandState
 * Signature: ([ILPSim/PostFlopState;)V
 */
JNIEXPORT void JNICALL Java_PSim_PSim_GetHandState___3ILPSim_PostFlopState_2 (JNIEnv* env, jclass cls, jintArray hand, jobject results)
{
    //Get the array into C.
    jint buf[7];
    env->GetIntArrayRegion(hand, 0, 7, buf);

    //Copy over so we have an int array.
    int cards[7];
    for (int i = 0; i < 7; ++i)
        cards[i] = buf[i];

    PostFlopState p;
    GetHandState(cards, &p);

    //Set the results back into java.
    ResultsPFS(p, results, env);
}


/*
 * Class:     PSim_PSim
 * Method:    RankHand
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_PSim_PSim_RankHand___3I (JNIEnv* env, jclass cls, jintArray hand)
{
    //Get the array into C.
    jint buf[7];
    env->GetIntArrayRegion(hand, 0, 7, buf);

    //Copy over so we have an int array.
    int cards[7];
    for (int i = 0; i < 7; ++i)
        cards[i] = buf[i];


    return RankHand(cards);
}


/*
 * Class:     PSim_PSim
 * Method:    SimulateHand
 * Signature: ([ILPSim/SimResults;FFI)V
 */
JNIEXPORT void JNICALL Java_PSim_PSim_SimulateHand___3ILPSim_SimResults_2FFI (JNIEnv* env, jclass cls, jintArray hand, jobject results, jfloat a, jfloat b, jint c)
{
    //Get the array into C.
    jint buf[7];
    env->GetIntArrayRegion(hand, 0, 7, buf);

    //Copy over so we have an int array.
    int cards[7];
    for (int i = 0; i < 7; ++i)
        cards[i] = buf[i];

    //Run the simulation.
    SimResults r;
    SimulateHand(cards, &r, a, b, c);

    //Set the results back into java.
    SetResultsSR(r, results, env);
}


/*
 * Class:     PSim_PSim
 * Method:    SimulateHandMulti
 * Signature: ([ILPSim/SimResults;III)V
 */
JNIEXPORT void JNICALL Java_PSim_PSim_SimulateHandMulti___3ILPSim_SimResults_2III (JNIEnv* env, jclass cls, jintArray hand, jobject results, jint a, jint b, jint c)
{
    //Get the array into C.
    jint buf[7];
    env->GetIntArrayRegion(hand, 0, 7, buf);

    //Copy over so we have an int array.
    int cards[7];
    for (int i = 0; i < 7; ++i)
        cards[i] = buf[i];

    //Run the simulation.
    SimResults r;
    SimulateHandMulti(cards, &r, a, b, c);

    //Set the results back into java.
    SetResultsSR(r, results, env);
}


/*
 * Class:     PSim_PSim
 * Method:    GetHandStateBrief
 * Signature: (LPSim/PostFlopState;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_PSim_PSim_GetHandStateBrief (JNIEnv* env, jclass cls, jobject results)
{
    PostFlopState p;
    ResultsPFS(p, results, env, false);

    const char* brief = GetHandStateBrief(&p);

    jstring jstr = env->NewStringUTF(brief);

    return jstr;
}

