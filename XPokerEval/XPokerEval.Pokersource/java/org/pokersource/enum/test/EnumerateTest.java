// $Id: EnumerateTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class EnumerateTest extends TestCase {
  private int[][] pocketRanks;
  private int[][] pocketSuits;
  private int[] boardRanks;
  private int[] boardSuits;
  private int[] deadRanks;
  private int[] deadSuits;
  private long[] pockets;
  private long board;
  private long dead;
  
  public EnumerateTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(EnumerateTest.class);
  }
  protected void setUp() {
    pocketRanks = new int[3][2];
    pocketSuits = new int[3][2];
    boardRanks = new int[3];
    boardSuits = new int[3];
    deadRanks = new int[2];
    deadSuits = new int[2];
    pockets = new long[3];

    // player 0 has Ks Kh
    pocketRanks[0][0] = Deck.RANK_KING;
    pocketSuits[0][0] = Deck.SUIT_SPADES;
    pocketRanks[0][1] = Deck.RANK_KING;
    pocketSuits[0][1] = Deck.SUIT_HEARTS;
    pockets[0] = Deck.createCardMask(pocketRanks[0], pocketSuits[0]);

    // player 1 has Ad Td
    pocketRanks[1][0] = Deck.RANK_ACE;
    pocketSuits[1][0] = Deck.SUIT_DIAMONDS;
    pocketRanks[1][1] = Deck.RANK_TEN;
    pocketSuits[1][1] = Deck.SUIT_DIAMONDS;
    pockets[1] = Deck.createCardMask(pocketRanks[1], pocketSuits[1]);

    // player 2 has 9c 8c
    pocketRanks[2][0] = Deck.RANK_9;
    pocketSuits[2][0] = Deck.SUIT_CLUBS;
    pocketRanks[2][1] = Deck.RANK_8;
    pocketSuits[2][1] = Deck.SUIT_CLUBS;
    pockets[2] = Deck.createCardMask(pocketRanks[2], pocketSuits[2]);

    // the board is Kd Jd Th
    boardRanks[0] = Deck.RANK_KING;
    boardSuits[0] = Deck.SUIT_DIAMONDS;
    boardRanks[1] = Deck.RANK_JACK;
    boardSuits[1] = Deck.SUIT_DIAMONDS;
    boardRanks[2] = Deck.RANK_TEN;
    boardSuits[2] = Deck.SUIT_HEARTS;
    board = Deck.createCardMask(boardRanks, boardSuits);

    // another player folded As 2h
    deadRanks[0] = Deck.RANK_ACE;
    deadSuits[0] = Deck.SUIT_SPADES;
    deadRanks[1] = Deck.RANK_2;
    deadSuits[1] = Deck.SUIT_HEARTS;
    dead = Deck.createCardMask(deadRanks, deadSuits);
  }

	public void testPotEquity()
	{
		// Compare to "pokenum -h ks kh ad td 9c 8c -- kd jd th / As 2h".
		double[] ev = new double[pocketRanks.length];
		Enumerate.PotEquity(Enumerate.GAME_HOLDEM, 0, pocketRanks, pocketSuits,
							boardRanks, boardSuits, deadRanks, deadSuits, ev);
		assertEquals(0.531707317073, ev[0], 1e-10);
		assertEquals(0.392682926829, ev[1], 1e-10);
		assertEquals(0.075609756098, ev[2], 1e-10);

		ev = new double[pockets.length];
		Enumerate.PotEquity(Enumerate.GAME_HOLDEM, 0, pockets, board, dead, ev);
		assertEquals(0.531707317073, ev[0], 1e-10);
		assertEquals(0.392682926829, ev[1], 1e-10);
		assertEquals(0.075609756098, ev[2], 1e-10);
	}
}
