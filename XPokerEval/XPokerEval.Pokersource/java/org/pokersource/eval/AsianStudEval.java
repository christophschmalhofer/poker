// $Id: AsianStudEval.java,v 1.5 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.eval;
import org.pokersource.game.Deck;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class AsianStudEval {
  static {
    System.loadLibrary("poker");
    System.loadLibrary("pokerjni");
  }

  /** Evaluate an asian stud deck poker hand for high.
      @param ranks ranks[i] is rank of ith card
      @param suits suits[i] is suit of ith card
      @return integer code denoting hand value (higher beats lower)
  */
  public static native long EvalHigh(int[] ranks, int[] suits);
  
  /** A simple demonstration of AsianStudEval methods. */
  public static void main(String[] args) {
    int[] ranks = new int[5];
    int[] suits = new int[5];
    ranks[0] = Deck.RANK_ACE;	suits[0] = Deck.SUIT_HEARTS;
    ranks[1] = Deck.RANK_7;	suits[1] = Deck.SUIT_HEARTS;
    ranks[2] = Deck.RANK_8;	suits[2] = Deck.SUIT_HEARTS;
    ranks[3] = Deck.RANK_9;	suits[3] = Deck.SUIT_SPADES;
    ranks[4] = Deck.RANK_TEN;	suits[4] = Deck.SUIT_SPADES;
    long mask = Deck.createCardMask(ranks, suits);
    System.out.println("In Java: cards=[" + Deck.cardMaskString(mask) + "]");
    try {
      long hival = EvalHigh(ranks, suits);
      System.out.println("In Java: hival=" + hival + "\n");
    } catch (Exception e) {
      System.out.println("In Java: caught exception: " + e);
    }
  }
}
