// $Id: HoldemBeliefVectorTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemBeliefVectorTest extends TestCase {
  HoldemBeliefVector v1;
  HoldemBeliefVector v2;
  HoldemBeliefVector v3;
  HoldemBeliefVector v4;

  public HoldemBeliefVectorTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(HoldemBeliefVectorTest.class);
  }

  protected void setUp() {
    v1 = new HoldemBeliefVector("AhKh");

    v2 = new HoldemBeliefVector("T9 52s:50");

    v3 = new HoldemBeliefVector("T9 52s:50");
    long dead = Deck.parseCardMask("5h 5s");
    v3.setDeadCards(dead);

    v4 = new HoldemBeliefVector("AA=50 SM2=50");
  }

  public void testGetHands() {
    assertEquals(1, v1.getHands().length);
    assertEquals(12+4, v2.getHands().length);
    assertEquals(12+4-2, v3.getHands().length);
    assertEquals(6+30, v4.getHands().length);
  }

  public void testGetBeliefProb() {
    double n1 = 1;
    assertEquals(1.0/n1, v1.getBeliefProb(Deck.parseCardMask("AhKh")), 1e-10);
    assertEquals(0.0/n1, v1.getBeliefProb(Deck.parseCardMask("AsKs")), 1e-10);

    double n2 = 12 + 4/2;
    assertEquals(0.0/n2, v2.getBeliefProb(Deck.parseCardMask("Th9h")), 1e-10);
    assertEquals(1.0/n2, v2.getBeliefProb(Deck.parseCardMask("Th9c")), 1e-10);
    assertEquals(0.5/n2, v2.getBeliefProb(Deck.parseCardMask("5h2h")), 1e-10);
    assertEquals(0.5/n2, v2.getBeliefProb(Deck.parseCardMask("5c2c")), 1e-10);
    assertEquals(0.0/n2, v2.getBeliefProb(Deck.parseCardMask("5h2c")), 1e-10);

    double n3 = 12 + (4-2)/2;
    assertEquals(0.0/n3, v3.getBeliefProb(Deck.parseCardMask("Th9h")), 1e-10);
    assertEquals(1.0/n3, v3.getBeliefProb(Deck.parseCardMask("Th9c")), 1e-10);
    assertEquals(0.0/n3, v3.getBeliefProb(Deck.parseCardMask("5h2h")), 1e-10);
    assertEquals(0.5/n3, v3.getBeliefProb(Deck.parseCardMask("5c2c")), 1e-10);
    assertEquals(0.0/n3, v3.getBeliefProb(Deck.parseCardMask("5h2c")), 1e-10);

    double n4AA = 6;
    double n4SM2 = 30;
    assertEquals(0.5/n4AA, v4.getBeliefProb(Deck.parseCardMask("AhAd")), 1e-10);
    assertEquals(0.5/n4SM2, v4.getBeliefProb(Deck.parseCardMask("KhQh")), 1e-10);
  }

}
