// $Id: AsianStudEvalTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.eval.test;
import org.pokersource.eval.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class AsianStudEvalTest extends TestCase {
  private int[] ranks;
  private int[] suits;
  
  public AsianStudEvalTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(AsianStudEvalTest.class);
  }
  protected void setUp() {
    ranks = new int[5];
    suits = new int[5];
    ranks[0] = Deck.RANK_ACE;	suits[0] = Deck.SUIT_HEARTS;
    ranks[1] = Deck.RANK_7;	suits[1] = Deck.SUIT_HEARTS;
    ranks[2] = Deck.RANK_8;	suits[2] = Deck.SUIT_HEARTS;
    ranks[3] = Deck.RANK_9;	suits[3] = Deck.SUIT_SPADES;
    ranks[4] = Deck.RANK_TEN;	suits[4] = Deck.SUIT_SPADES;
  }

  public void testEvalHigh() {
    long hival = AsianStudEval.EvalHigh(ranks, suits);
    assertEquals(67633152, hival);
  }
}
