// $Id: AllTests.java,v 1.5 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class AllTests {
  public static void main (String[] args) {
    junit.textui.TestRunner.run(suite());
  }
  public static Test suite() {
    TestSuite suite = new TestSuite("All org.pokersource.enum tests");
    suite.addTest(new TestSuite(EnumerateTest.class));
    suite.addTest(new TestSuite(HandMatchupTest.class));
    suite.addTest(new TestSuite(HoldemAbdulGroupTest.class));
    suite.addTest(new TestSuite(HoldemAtomicGroupTest.class));
    suite.addTest(new TestSuite(HoldemBeliefVectorTest.class));
    suite.addTest(new TestSuite(HoldemCanonGroupTest.class));
    suite.addTest(new TestSuite(HoldemHandGroupFactoryTest.class));
    suite.addTest(new TestSuite(HoldemSMGroupTest.class));
    suite.addTest(new TestSuite(HoldemUniversalGroupTest.class));
    suite.addTest(new TestSuite(NestedLoopEnumerationTest.class));
    suite.addTest(new TestSuite(SAIETest.class));
    return suite;
  }
}
