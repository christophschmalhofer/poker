// $Id: HoldemSMGroupTest.java,v 1.4 2002/06/19 08:22:13 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemSMGroupTest extends TestCase {
  private HoldemSMGroup gSM1;
  private HoldemSMGroup gSM2;
  private HoldemSMGroup gSM3;
  private HoldemSMGroup gSM4;
  private HoldemSMGroup gSM5;
  private HoldemSMGroup gSM6;
  private HoldemSMGroup gSM7;
  private HoldemSMGroup gSM8;
  private HoldemSMGroup gSM9;
  private long AhAd;
  private long KhQh;
  private long KhQs;
  private long Td9d;
  private long Td9c;
  private long _7d3c;

  
  public HoldemSMGroupTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemSMGroupTest.class);
  }

  protected void setUp() {
    gSM1 = new HoldemSMGroup("SM1");
    gSM2 = new HoldemSMGroup("SM2");
    gSM3 = new HoldemSMGroup("SM3");
    gSM4 = new HoldemSMGroup("SM4");
    gSM5 = new HoldemSMGroup("SM5");
    gSM6 = new HoldemSMGroup("SM6");
    gSM7 = new HoldemSMGroup("SM7");
    gSM8 = new HoldemSMGroup("SM8");
    gSM9 = new HoldemSMGroup("SM9");
    AhAd = Deck.parseCardMask("AhAd");
    KhQh = Deck.parseCardMask("KhQh");
    KhQs = Deck.parseCardMask("KhQs");
    Td9d = Deck.parseCardMask("Td9d");
    Td9c = Deck.parseCardMask("Td9c");
    _7d3c = Deck.parseCardMask("7d3c");
  }

  public void testToString() {
    assertEquals("SM1", gSM1.toString());
    assertEquals("SM2", gSM2.toString());
    assertEquals("SM9", gSM9.toString());
  }
  
  public void testIsHandInGroup() {
    assertTrue(gSM1.isHandInGroup(AhAd));
    assertTrue(!gSM1.isHandInGroup(KhQh));
    assertTrue(!gSM1.isHandInGroup(KhQs));
    assertTrue(!gSM1.isHandInGroup(Td9d));
    assertTrue(!gSM1.isHandInGroup(Td9c));
    assertTrue(!gSM1.isHandInGroup(_7d3c));

    assertTrue(!gSM2.isHandInGroup(AhAd));
    assertTrue(gSM2.isHandInGroup(KhQh));
    assertTrue(!gSM2.isHandInGroup(KhQs));
    assertTrue(!gSM2.isHandInGroup(Td9d));
    assertTrue(!gSM2.isHandInGroup(Td9c));
    assertTrue(!gSM2.isHandInGroup(_7d3c));

    assertTrue(!gSM9.isHandInGroup(AhAd));
    assertTrue(!gSM9.isHandInGroup(KhQh));
    assertTrue(!gSM9.isHandInGroup(KhQs));
    assertTrue(!gSM9.isHandInGroup(Td9d));
    assertTrue(!gSM9.isHandInGroup(Td9c));
    assertTrue(gSM9.isHandInGroup(_7d3c));
  }
  
  public void testGetHands() {
    // 6+6+6+6+4 = 28
    assertEquals(28, gSM1.getHands().length);

    // 6+4+4+4+12 = 30
    assertEquals(30, gSM2.getHands().length);

    // 6+4+4+4+4+12 = 34
    assertEquals(34, gSM3.getHands().length);

    // 4+12+6+4+4+4+12+4 = 50
    assertEquals(50, gSM4.getHands().length);

    // 6+4+4+4+12+12+12+4+4+4+4+4+4+4+4+4+4+4 = 98
    assertEquals(98, gSM5.getHands().length);

    // 6+12+6+4+12+12+4+4+4 = 64
    assertEquals(64, gSM6.getHands().length);

    // 6+12+4+4+12+6+12+4+6+4+4+4+4+4+4+4+4+4 = 102
    assertEquals(102, gSM7.getHands().length);

    // 12+4+12+12+12+4+4+4+4+12+4+12+12+4+12+12 = 136
    assertEquals(136, gSM8.getHands().length);

    // 1326 - (28+30+34+50+98+64+102+136) = 784
    assertEquals(784, gSM9.getHands().length);
  }

  public void testDisjoint() {
    HoldemSMGroup[] groups = {gSM1, gSM2, gSM3, gSM4, gSM5,
                              gSM6, gSM7, gSM8, gSM9};
    for (int i=0; i<groups.length; i++) {
      long[] hands = groups[i].getHands();
      for (int j=0; j<hands.length; j++) {
        for (int k=0; k<groups.length; k++) {
          boolean isSameGroup = (i == k);
          boolean isMember = groups[k].isHandInGroup(hands[j]);
          assertTrue(isSameGroup == isMember);
        }
      }
    }
  }
    
}
