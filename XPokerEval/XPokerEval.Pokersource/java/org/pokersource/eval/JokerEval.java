// $Id: JokerEval.java,v 1.5 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.eval;
import org.pokersource.game.Deck;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class JokerEval {
  static {
    System.loadLibrary("poker");
    System.loadLibrary("pokerjni");
  }

  /** Evaluate a joker deck poker hand for high.
      @param ranks ranks[i] is rank of ith card
      @param suits suits[i] is suit of ith card
      @return integer code denoting hand value (higher beats lower)
  */
  public static native long EvalHigh(int[] ranks, int[] suits);

  /** Evaluate a joker deck poker hand for low (no qualifier).
      @param ranks ranks[i] is rank of ith card
      @param suits suits[i] is suit of ith card
      @return integer code denoting hand value (higher beats lower)
  */
  public static native long EvalLow(int[] ranks, int[] suits);

  /** Evaluate a joker deck poker hand for low (8 qualifier).
      @param ranks ranks[i] is rank of ith card
      @param suits suits[i] is suit of ith card
      @return integer code denoting hand value (higher beats lower)
  */
  public static native long EvalLow8(int[] ranks, int[] suits);
  
  /** A simple demonstration of JokerEval methods. */
  public static void main(String[] args) {
    int[] ranks = new int[5];
    int[] suits = new int[5];
    ranks[0] = Deck.RANK_3;		suits[0] = Deck.SUIT_HEARTS;
    ranks[1] = Deck.RANK_4;		suits[1] = Deck.SUIT_HEARTS;
    ranks[2] = Deck.RANK_5;		suits[2] = Deck.SUIT_HEARTS;
    ranks[3] = Deck.RANK_JOKER;		suits[3] = Deck.SUIT_JOKER;
    ranks[4] = Deck.RANK_7;		suits[4] = Deck.SUIT_HEARTS;
    long mask = Deck.createCardMask(ranks, suits);
    System.out.println("In Java: cards=[" + Deck.cardMaskString(mask) + "]");
    try {
      long hival = EvalHigh(ranks, suits);
      System.out.println("In Java: hival=" + hival + "\n");
      long loval = EvalLow(ranks, suits);
      System.out.println("In Java: loval=" + loval + "\n");
      long lo8val = EvalLow8(ranks, suits);
      System.out.println("In Java: lo8val=" + lo8val + "\n");
    } catch (Exception e) {
      System.out.println("In Java: caught exception: " + e);
    }
  }
}
