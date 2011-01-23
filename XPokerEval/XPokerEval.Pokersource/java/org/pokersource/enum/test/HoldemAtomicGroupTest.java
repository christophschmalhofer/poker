// $Id: HoldemAtomicGroupTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemAtomicGroupTest extends TestCase {
  private HoldemAtomicGroup gAhAd;
  private HoldemAtomicGroup gKhQh;
  private HoldemAtomicGroup gTd9c;
  private long AhAd;
  private long KhQh;
  private long KhQs;
  private long Td9d;
  private long Td9c;
  
  public HoldemAtomicGroupTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemAtomicGroupTest.class);
  }
  protected void setUp() {
    gAhAd = new HoldemAtomicGroup("AhAd");
    gKhQh = new HoldemAtomicGroup("KhQh");
    gTd9c = new HoldemAtomicGroup("Td9c");
    AhAd = Deck.parseCardMask("AhAd");
    KhQh = Deck.parseCardMask("KhQh");
    KhQs = Deck.parseCardMask("KhQs");
    Td9d = Deck.parseCardMask("Td9d");
    Td9c = Deck.parseCardMask("Td9c");
  }

  public void testToString() {
    assertEquals("AhAd", gAhAd.toString());
    assertEquals("KhQh", gKhQh.toString());
    assertEquals("Td9c", gTd9c.toString());
  }
  
  public void testIsHandInGroup() {
    assertTrue(gAhAd.isHandInGroup(AhAd));
    assertTrue(!gAhAd.isHandInGroup(KhQh));
    assertTrue(!gAhAd.isHandInGroup(KhQs));
    assertTrue(!gAhAd.isHandInGroup(Td9d));
    assertTrue(!gAhAd.isHandInGroup(Td9c));

    assertTrue(!gKhQh.isHandInGroup(AhAd));
    assertTrue(gKhQh.isHandInGroup(KhQh));
    assertTrue(!gKhQh.isHandInGroup(KhQs));
    assertTrue(!gKhQh.isHandInGroup(Td9d));
    assertTrue(!gKhQh.isHandInGroup(Td9c));

    assertTrue(!gTd9c.isHandInGroup(AhAd));
    assertTrue(!gTd9c.isHandInGroup(KhQh));
    assertTrue(!gTd9c.isHandInGroup(KhQs));
    assertTrue(!gTd9c.isHandInGroup(Td9d));
    assertTrue(gTd9c.isHandInGroup(Td9c));
  }
  
  public void testGetHands() {
    assertEquals(1, gAhAd.getHands().length);
    assertEquals(1, gKhQh.getHands().length);
    assertEquals(1, gTd9c.getHands().length);
  }

}
