package handevaluator.tests;

import java.io.File;
import java.io.IOException;

import handevaluator.Deck;
import handevaluator.HandEvaluator;
import handevaluator.classes.EquivalenceClasses;
import handevaluator.jni.JNIHandEvaluator;
import handevaluator.purejava.OneStagePJHE;
import handevaluator.purejava.TwoStagePJHE;

public class BulkTests {
	private static final int TRIALS = 15000000;
	private static final File DAGFILE = new File("carddag");

	public static void runtests(HandEvaluator he, int trials) {
		Deck d = new Deck();
		long ts = System.currentTimeMillis();
		for (int i=0; i < trials; i++) {
			if ((i+1) % 1000000 == 0) {
				System.out.println((System.currentTimeMillis() - ts) + "ms, evaluated " + ((i+1) / 1000000) + " million hands");
			}
			d.randomHand();
			he.calculateEquivalenceClass(d.getBytes());
		}
		System.out.println(he.getClass().getName() + ", " + trials + " trials: " + (System.currentTimeMillis() - ts) + "ms elapsed");
	}
	
	public static void overhead(int trials) {
		Deck d = new Deck();
		long ts = System.currentTimeMillis();
		for (int i=0; i < trials; i++) {
			if ((i+1) % 1000000 == 0) {
				System.out.println((System.currentTimeMillis() - ts) + "ms, evaluated " + ((i+1) / 1000000) + " million hands");
			}
			d.randomHand();
		}
		System.out.println("Overhead of " + trials + " trials: " + (System.currentTimeMillis() - ts) + "ms elapsed");		
	}
	
	public static void runNativeTests(int t) {
		long ts = System.currentTimeMillis();
		JNIHandEvaluator.runOverhead(t);
		System.out.println("Native overhead: " + (System.currentTimeMillis() - ts) + "ms");
		ts = System.currentTimeMillis();
		JNIHandEvaluator.runSuite(t);
		System.out.println("Native test: " + (System.currentTimeMillis() - ts) + "ms");
	}
	
	public static void main(String[] args) throws IOException {
		EquivalenceClasses.getInstance();	// assert equivalence class loading
		BulkTests.runNativeTests(TRIALS);
		BulkTests.runtests(new JNIHandEvaluator(), TRIALS);
		BulkTests.overhead(TRIALS);
		BulkTests.runtests(new OneStagePJHE(DAGFILE), TRIALS);
		BulkTests.runtests(new TwoStagePJHE(DAGFILE), TRIALS);
	}	
}
