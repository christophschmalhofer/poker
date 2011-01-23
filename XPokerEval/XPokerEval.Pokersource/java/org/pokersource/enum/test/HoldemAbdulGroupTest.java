// $Id: HoldemAbdulGroupTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemAbdulGroupTest extends TestCase {
  private HoldemAbdulGroup g99;
  private HoldemAbdulGroup gATs;
  private HoldemAbdulGroup gKTs;
  private HoldemAbdulGroup gQTs;
  private HoldemAbdulGroup g9xs;
  private HoldemAbdulGroup g76s;
  private HoldemAbdulGroup g75s;
  private HoldemAbdulGroup g74s;
  private HoldemAbdulGroup gAT;
  private HoldemAbdulGroup gKT;
  private HoldemAbdulGroup gQT;
  private HoldemAbdulGroup g9x;
  private HoldemAbdulGroup g76;
  private HoldemAbdulGroup g75;
  private HoldemAbdulGroup g74;
  
  public HoldemAbdulGroupTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemAbdulGroupTest.class);
  }

  protected void setUp() {
    g99 = new HoldemAbdulGroup("99+");
    gATs = new HoldemAbdulGroup("ATs+");
    gKTs = new HoldemAbdulGroup("KTs+");
    gQTs = new HoldemAbdulGroup("QTs+");
    g9xs = new HoldemAbdulGroup("9xs+");
    g76s = new HoldemAbdulGroup("76s+");
    g75s = new HoldemAbdulGroup("75s+");
    g74s = new HoldemAbdulGroup("74s+");
    gAT = new HoldemAbdulGroup("AT+");
    gKT = new HoldemAbdulGroup("KT+");
    gQT = new HoldemAbdulGroup("QT+");
    g9x = new HoldemAbdulGroup("9x+");
    g76 = new HoldemAbdulGroup("76+");
    g75 = new HoldemAbdulGroup("75+");
    g74 = new HoldemAbdulGroup("74+");
  }

  public void testToString() {
    assertEquals("99+", g99.toString());
    assertEquals("ATs+", gATs.toString());
    assertEquals("KTs+", gKTs.toString());
    assertEquals("QTs+", gQTs.toString());
    assertEquals("9xs+", g9xs.toString());
    assertEquals("76s+", g76s.toString());
    assertEquals("75s+", g75s.toString());
    assertEquals("74s+", g74s.toString());
    assertEquals("AT+", gAT.toString());
    assertEquals("KT+", gKT.toString());
    assertEquals("QT+", gQT.toString());
    assertEquals("9x+", g9x.toString());
    assertEquals("76+", g76.toString());
    assertEquals("75+", g75.toString());
    assertEquals("74+", g74.toString());
  }
  
  public void testIsHandInGroup() {
    assertTrue(g99.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!g99.isHandInGroup(Deck.parseCardMask("8h8d")));
    assertTrue(!g99.isHandInGroup(Deck.parseCardMask("AhKh")));
    assertTrue(!g99.isHandInGroup(Deck.parseCardMask("Td9c")));

    assertTrue(!gATs.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(gATs.isHandInGroup(Deck.parseCardMask("AhTh")));
    assertTrue(!gATs.isHandInGroup(Deck.parseCardMask("Ah9h")));
    assertTrue(!gATs.isHandInGroup(Deck.parseCardMask("AhTd")));
    assertTrue(!gATs.isHandInGroup(Deck.parseCardMask("Ah9d")));

    assertTrue(!gKTs.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(gKTs.isHandInGroup(Deck.parseCardMask("KhTh")));
    assertTrue(!gKTs.isHandInGroup(Deck.parseCardMask("Kh9h")));
    assertTrue(!gKTs.isHandInGroup(Deck.parseCardMask("KhTd")));
    assertTrue(!gKTs.isHandInGroup(Deck.parseCardMask("Kh9d")));

    assertTrue(!gQTs.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(gQTs.isHandInGroup(Deck.parseCardMask("QhTh")));
    assertTrue(!gQTs.isHandInGroup(Deck.parseCardMask("Qh9h")));
    assertTrue(!gQTs.isHandInGroup(Deck.parseCardMask("QhTd")));
    assertTrue(!gQTs.isHandInGroup(Deck.parseCardMask("Qh9d")));

    assertTrue(!g9xs.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(g9xs.isHandInGroup(Deck.parseCardMask("9h8h")));
    assertTrue(g9xs.isHandInGroup(Deck.parseCardMask("9h2h")));
    assertTrue(!g9xs.isHandInGroup(Deck.parseCardMask("9h8d")));
    assertTrue(!g9xs.isHandInGroup(Deck.parseCardMask("9h2d")));
    assertTrue(!g9xs.isHandInGroup(Deck.parseCardMask("Th9h")));
    assertTrue(!g9xs.isHandInGroup(Deck.parseCardMask("Th9d")));

    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("QhJh")));
    assertTrue(g76s.isHandInGroup(Deck.parseCardMask("JhTh")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("JhTc")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("Jh9h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("Jh9c")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("Jh8h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("Jh8c")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("Jh7h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("Jh7c")));
    assertTrue(g76s.isHandInGroup(Deck.parseCardMask("7h6h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("7h6c")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("7h5h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("7h5c")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("7h4h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("7h4c")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("7h3h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("7h3c")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("6h5h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("6h4h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("6h3h")));
    assertTrue(!g76s.isHandInGroup(Deck.parseCardMask("6h2h")));

    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("QhJh")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("JhTh")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("JhTc")));
    assertTrue(g75s.isHandInGroup(Deck.parseCardMask("Jh9h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("Jh9c")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("Jh8h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("Jh8c")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("Jh7h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("Jh7c")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("7h6h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("7h6c")));
    assertTrue(g75s.isHandInGroup(Deck.parseCardMask("7h5h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("7h5c")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("7h4h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("7h4c")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("7h3h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("7h3c")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("6h5h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("6h4h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("6h3h")));
    assertTrue(!g75s.isHandInGroup(Deck.parseCardMask("6h2h")));

    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("QhJh")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("JhTh")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("JhTc")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("Jh9h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("Jh9c")));
    assertTrue(g74s.isHandInGroup(Deck.parseCardMask("Jh8h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("Jh8c")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("Jh7h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("Jh7c")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("7h6h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("7h6c")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("7h5h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("7h5c")));
    assertTrue(g74s.isHandInGroup(Deck.parseCardMask("7h4h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("7h4c")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("7h3h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("7h3c")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("6h5h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("6h4h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("6h3h")));
    assertTrue(!g74s.isHandInGroup(Deck.parseCardMask("6h2h")));

    assertTrue(!gAT.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!gAT.isHandInGroup(Deck.parseCardMask("AhTh")));
    assertTrue(!gAT.isHandInGroup(Deck.parseCardMask("Ah9h")));
    assertTrue(gAT.isHandInGroup(Deck.parseCardMask("AhTd")));
    assertTrue(!gAT.isHandInGroup(Deck.parseCardMask("Ah9d")));

    assertTrue(!gKT.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!gKT.isHandInGroup(Deck.parseCardMask("KhTh")));
    assertTrue(!gKT.isHandInGroup(Deck.parseCardMask("Kh9h")));
    assertTrue(gKT.isHandInGroup(Deck.parseCardMask("KhTd")));
    assertTrue(!gKT.isHandInGroup(Deck.parseCardMask("Kh9d")));

    assertTrue(!gQT.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!gQT.isHandInGroup(Deck.parseCardMask("QhTh")));
    assertTrue(!gQT.isHandInGroup(Deck.parseCardMask("Qh9h")));
    assertTrue(gQT.isHandInGroup(Deck.parseCardMask("QhTd")));
    assertTrue(!gQT.isHandInGroup(Deck.parseCardMask("Qh9d")));

    assertTrue(!g9x.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!g9x.isHandInGroup(Deck.parseCardMask("9h8h")));
    assertTrue(!g9x.isHandInGroup(Deck.parseCardMask("9h2h")));
    assertTrue(g9x.isHandInGroup(Deck.parseCardMask("9h8d")));
    assertTrue(g9x.isHandInGroup(Deck.parseCardMask("9h2d")));
    assertTrue(!g9x.isHandInGroup(Deck.parseCardMask("Th9h")));
    assertTrue(!g9x.isHandInGroup(Deck.parseCardMask("Th9d")));

    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("QhJh")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("JhTh")));
    assertTrue(g76.isHandInGroup(Deck.parseCardMask("JhTc")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("Jh9h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("Jh9c")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("Jh8h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("Jh8c")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("Jh7h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("Jh7c")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("7h6h")));
    assertTrue(g76.isHandInGroup(Deck.parseCardMask("7h6c")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("7h5h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("7h5c")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("7h4h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("7h4c")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("7h3h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("7h3c")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("6h5h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("6h4h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("6h3h")));
    assertTrue(!g76.isHandInGroup(Deck.parseCardMask("6h2h")));

    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("QhJh")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("JhTh")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("JhTc")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("Jh9h")));
    assertTrue(g75.isHandInGroup(Deck.parseCardMask("Jh9c")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("Jh8h")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("Jh8c")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("Jh7h")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("Jh7c")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("7h6h")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("7h6c")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("7h5h")));
    assertTrue(g75.isHandInGroup(Deck.parseCardMask("7h5c")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("7h4h")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("7h4c")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("7h3h")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("7h3c")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("6h5h")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("6h4h")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("6h3h")));
    assertTrue(!g75.isHandInGroup(Deck.parseCardMask("6h2h")));

    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("9h9d")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("QhJh")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("JhTh")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("JhTc")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("Jh9h")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("Jh9c")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("Jh8h")));
    assertTrue(g74.isHandInGroup(Deck.parseCardMask("Jh8c")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("Jh7h")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("Jh7c")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("7h6h")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("7h6c")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("7h5h")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("7h5c")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("7h4h")));
    assertTrue(g74.isHandInGroup(Deck.parseCardMask("7h4c")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("7h3h")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("7h3c")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("6h5h")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("6h4h")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("6h3h")));
    assertTrue(!g74.isHandInGroup(Deck.parseCardMask("6h2h")));

  }
  
  public void testGetHands() {
    assertEquals(6*6, g99.getHands().length);
    assertEquals(4*4, gATs.getHands().length);
    assertEquals(3*4, gKTs.getHands().length);
    assertEquals(2*4, gQTs.getHands().length);
    assertEquals(7*4, g9xs.getHands().length);
    assertEquals(5*4, g76s.getHands().length);
    assertEquals(5*4, g75s.getHands().length);
    assertEquals(5*4, g74s.getHands().length);
    assertEquals(4*12, gAT.getHands().length);
    assertEquals(3*12, gKT.getHands().length);
    assertEquals(2*12, gQT.getHands().length);
    assertEquals(7*12, g9x.getHands().length);
    assertEquals(5*12, g76.getHands().length);
    assertEquals(5*12, g75.getHands().length);
    assertEquals(5*12, g74.getHands().length);
  }

}
