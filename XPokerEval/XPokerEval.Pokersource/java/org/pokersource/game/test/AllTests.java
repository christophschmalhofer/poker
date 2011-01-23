// $Id: AllTests.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.game.test;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class AllTests {
  public static void main (String[] args) {
    junit.textui.TestRunner.run(suite());
  }
  public static Test suite() {
    TestSuite suite = new TestSuite("All org.pokersource.game tests");
    suite.addTest(new TestSuite(DeckTest.class));
    return suite;
  }
}
