// $Id: HoldemUniversalGroupTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemUniversalGroupTest extends TestCase {
  private HoldemUniversalGroup univ;
  private long AhAd;
  private long KhQh;
  private long KhQs;
  private long Td9d;
  private long Td9c;
  
  public HoldemUniversalGroupTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemUniversalGroupTest.class);
  }
  protected void setUp() {
    univ = new HoldemUniversalGroup("<any>");
    AhAd = Deck.parseCardMask("AhAd");
    KhQh = Deck.parseCardMask("KhQh");
    KhQs = Deck.parseCardMask("KhQs");
    Td9d = Deck.parseCardMask("Td9d");
    Td9c = Deck.parseCardMask("Td9c");
  }

  public void testToString() {
    assertEquals("<any>", univ.toString());
  }
  
  public void testIsHandInGroup() {
    assertTrue(univ.isHandInGroup(AhAd));
    assertTrue(univ.isHandInGroup(KhQh));
    assertTrue(univ.isHandInGroup(KhQs));
    assertTrue(univ.isHandInGroup(Td9d));
    assertTrue(univ.isHandInGroup(Td9c));
  }
  
  public void testGetHands() {
    assertEquals(1326, univ.getHands().length);
  }

}
