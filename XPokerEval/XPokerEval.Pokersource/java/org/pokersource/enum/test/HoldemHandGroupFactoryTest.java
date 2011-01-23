// $Id: HoldemHandGroupFactoryTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemHandGroupFactoryTest extends TestCase {
  public HoldemHandGroupFactoryTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemHandGroupFactoryTest.class);
  }

  protected void setUp() {
  }

  public void testCache() {
    HoldemHandGroup g1 = HoldemHandGroupFactory.getInstance("AK");
    HoldemHandGroup g2 = HoldemHandGroupFactory.getInstance("Q9s+");
    HoldemHandGroup g3 = HoldemHandGroupFactory.getInstance("AK");
    assertTrue(g1 != g2);
    assertTrue(!g1.equals(g2));
    assertTrue(g1 == g3);
    assertTrue(g1.equals(g3));
  }

  public void testException() {
    try {
      HoldemHandGroup g1 = HoldemHandGroupFactory.getInstance("JUNK");
      fail("Should raise IllegalArgumentException");
    } catch (IllegalArgumentException e) {
    } catch (RuntimeException e) {
      fail("Should raise IllegalArgumentException");
    }
  }
}
