// $Id: NestedLoopEnumerationTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class NestedLoopEnumerationTest extends TestCase {
  public NestedLoopEnumerationTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(NestedLoopEnumerationTest.class);
  }

  public void testBasic() {
    int[] limits = {2, 3, 2};
    int[][] expected = {
      {0, 0, 0},
      {0, 0, 1},
      {0, 1, 0},
      {0, 1, 1},
      {0, 2, 0},
      {0, 2, 1},
      {1, 0, 0},
      {1, 0, 1},
      {1, 1, 0},
      {1, 1, 1},
      {1, 2, 0},
      {1, 2, 1}
    };
    NestedLoopEnumeration enum = new NestedLoopEnumeration(limits);
    for (int i=0; i<expected.length; i++) {
      assertTrue(enum.hasMoreElements());
      int[] elem = (int[]) enum.nextElement();
      assertEquals(expected[i].length, elem.length);
      for (int j=0; j<expected[i].length; j++)
        assertEquals(expected[i][j], elem[j]);
    }
    assertTrue(!enum.hasMoreElements());
  }

}
