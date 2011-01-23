/* $Id: jniutil.c,v 1.1 2002/05/01 02:45:17 mjmaurer Exp $ */

#include <jni.h>

/* Try to throw an exception back to java.  This may fail, so anyone calling
   this function should assume that it may return. */
void
jniThrow(JNIEnv *env, jclass class, char *msg)
{
  jclass exClass;

  exClass = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
  if (exClass == 0) { /* Unable to find the new exception class, give up. */
    return;
  }
  (*env)->ThrowNew(env, exClass, msg);
}

