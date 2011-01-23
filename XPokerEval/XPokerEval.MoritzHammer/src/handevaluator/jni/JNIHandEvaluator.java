package handevaluator.jni;

import handevaluator.HandEvaluator;
import handevaluator.classes.EquivalenceClass;
import handevaluator.classes.EquivalenceClasses;

public class JNIHandEvaluator extends HandEvaluator {
	private static native int calculateClassIndex(byte[] hand);
	private static native void initializeJNI();
	
	static {
		System.loadLibrary("handeval");
		initializeJNI();
	}
	
	@Override
	public EquivalenceClass calculateEquivalenceClass(byte[] hand) {
		int idx = calculateClassIndex(hand);
		return EquivalenceClasses.getInstance().getEquivalenceClass(idx);
	}
	
	public static native void runSuite(int size);
	public static native void runOverhead(int size);
}
