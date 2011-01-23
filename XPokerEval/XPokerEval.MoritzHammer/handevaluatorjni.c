#import "handevaluator_jni_JNIHandEvaluator.h"
#import "handevaluator.h"
#import "carddeck.h"
#import "randomgenerator.h"

JNIEXPORT jint JNICALL Java_handevaluator_jni_JNIHandEvaluator_calculateClassIndex (JNIEnv *env, jclass clazz, jbyteArray cards) {
	jsize cardcnt = (*env)->GetArrayLength(env, cards);
    jbyte cardarr[cardcnt];
    (*env)->GetByteArrayRegion(env, cards, 0, cardcnt, cardarr);
    handeval_eq_class *c = calculate_equivalence_class((char*) cardarr);
    return (jint) c->id;
}


JNIEXPORT void JNICALL Java_handevaluator_jni_JNIHandEvaluator_initializeJNI(JNIEnv *env, jclass clazz) {
	handeval_init();
}

/**  These calls are used for comparing the speed of the Java implementation to 
 * the native one. I use JNI to trigger this to get the same wall-clock reading using System.currentTimeMillis. */
JNIEXPORT void JNICALL Java_handevaluator_jni_JNIHandEvaluator_runSuite(JNIEnv *env , jclass clazz, jint trials) {
	init_random(trials);
	deck_struct deck;
	init_deck(&deck);
	int i, j;
	char cards[7];
	for (i=0; i < trials; i++) {
		reset_deck(&deck);
		for (j=0; j < 7; j++) {
			cards[j] = draw_random_card(&deck); 
		}
		calculate_equivalence_class(cards);
	}
}

JNIEXPORT void JNICALL Java_handevaluator_jni_JNIHandEvaluator_runOverhead(JNIEnv *env , jclass clazz, jint trials) {
	init_random(trials);
	deck_struct deck;
	init_deck(&deck);
	int i, j;
	char cards[7];
	for (i=0; i < trials; i++) {
		reset_deck(&deck);
		for (j=0; j < 7; j++) {
			cards[j] = draw_random_card(&deck); 
		}
	}
}
