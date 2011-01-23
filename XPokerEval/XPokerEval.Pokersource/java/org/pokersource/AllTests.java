// $Id: AllTests.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource;
import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class AllTests {
  public static void main (String[] args) {
    junit.textui.TestRunner.run(suite());
  }
  public static Test suite() {
    TestSuite suite = new TestSuite("All org.pokersource tests");
    suite.addTest(org.pokersource.game.test.AllTests.suite());
    suite.addTest(org.pokersource.eval.test.AllTests.suite());
    suite.addTest(org.pokersource.enum.test.AllTests.suite());
    return suite;
  }
}
