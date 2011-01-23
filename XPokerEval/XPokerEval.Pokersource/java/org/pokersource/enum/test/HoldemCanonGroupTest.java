// $Id: HoldemCanonGroupTest.java,v 1.4 2002/06/19 08:18:52 mjmaurer Exp $

package org.pokersource.enum.test;
import java.util.Iterator;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemCanonGroupTest extends TestCase {
  private HoldemCanonGroup gAA;
  private HoldemCanonGroup gKQs;
  private HoldemCanonGroup gT9;
  private long AhAd;
  private long KhQh;
  private long KhQs;
  private long Td9d;
  private long Td9c;
  
  public HoldemCanonGroupTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemCanonGroupTest.class);
  }

  protected void setUp() {
    gAA = new HoldemCanonGroup("AA");
    gKQs = new HoldemCanonGroup("KQs");
    gT9 = new HoldemCanonGroup("T9");
    AhAd = Deck.parseCardMask("AhAd");
    KhQh = Deck.parseCardMask("KhQh");
    KhQs = Deck.parseCardMask("KhQs");
    Td9d = Deck.parseCardMask("Td9d");
    Td9c = Deck.parseCardMask("Td9c");
  }

  public void testToString() {
    assertEquals("AA", gAA.toString());
    assertEquals("KQs", gKQs.toString());
    assertEquals("T9", gT9.toString());
  }
  
  public void testIsHandInGroup() {
    assertTrue(gAA.isHandInGroup(AhAd));
    assertTrue(!gAA.isHandInGroup(KhQh));
    assertTrue(!gAA.isHandInGroup(KhQs));
    assertTrue(!gAA.isHandInGroup(Td9d));
    assertTrue(!gAA.isHandInGroup(Td9c));

    assertTrue(!gKQs.isHandInGroup(AhAd));
    assertTrue(gKQs.isHandInGroup(KhQh));
    assertTrue(!gKQs.isHandInGroup(KhQs));
    assertTrue(!gKQs.isHandInGroup(Td9d));
    assertTrue(!gKQs.isHandInGroup(Td9c));

    assertTrue(!gT9.isHandInGroup(AhAd));
    assertTrue(!gT9.isHandInGroup(KhQh));
    assertTrue(!gT9.isHandInGroup(KhQs));
    assertTrue(!gT9.isHandInGroup(Td9d));
    assertTrue(gT9.isHandInGroup(Td9c));
  }
  
  public void testGetHands() {
    assertEquals(6, gAA.getHands().length);
    assertEquals(4, gKQs.getHands().length);
    assertEquals(12, gT9.getHands().length);
  }

  public void testAllGroups() {
    int ngroups = 0;
    for (Iterator iter = HoldemCanonGroup.allGroups(); iter.hasNext();) {
      HoldemCanonGroup group = (HoldemCanonGroup) iter.next();
      //System.out.print(group.toString() + " ");
      ngroups++;
    }
    assertEquals(169, ngroups);
  }
    
}
