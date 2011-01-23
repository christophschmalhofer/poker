// $Id: DeckTest.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.game.test;
import org.pokersource.game.Deck;
import junit.framework.*;

/** Tests to ensure compatibility with the C-language macros.
    @author Michael Maurer <mjmaurer@yahoo.com>
 */

public class DeckTest extends TestCase {
  private int indexAs;
  private int indexAh;
  private int indexAd;
  private int indexAc;
  private int index2s;
  private int index2h;
  private int index2d;
  private int index2c;
  private int indexJoker;
  private long maskAs;
  private long maskAh;
  private long maskAd;
  private long maskAc;
  private long mask2s;
  private long mask2h;
  private long mask2d;
  private long mask2c;
  private long maskJoker;

  public DeckTest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(DeckTest.class);
  }

  protected void setUp() {
    // should match results of macros in deck_std.h
    indexAs = 3*13 + 12;
    indexAh = 0*13 + 12;
    indexAd = 1*13 + 12;
    indexAc = 2*13 + 12;
    index2s = 3*13 + 0;
    index2h = 0*13 + 0;
    index2d = 1*13 + 0;
    index2c = 2*13 + 0;
    maskAs = 1L << indexAs;
    maskAh = 1L << indexAh;
    maskAd = 1L << indexAd;
    maskAc = 1L << indexAc;
    mask2s = 1L << index2s;
    mask2h = 1L << index2h;
    mask2d = 1L << index2d;
    mask2c = 1L << index2c;
    // should match results of macros in deck_joker.h
    indexJoker = 52;
    maskJoker = 1L << indexJoker;
  }

  public void testCreateCardIndex() {
    // should match macro StdDeck_MAKE_CARD in deck_std.h
    assertEquals(index2s, Deck.createCardIndex(Deck.RANK_2, Deck.SUIT_SPADES));
    assertEquals(index2h, Deck.createCardIndex(Deck.RANK_2, Deck.SUIT_HEARTS));
    assertEquals(index2d, Deck.createCardIndex(Deck.RANK_2, Deck.SUIT_DIAMONDS));
    assertEquals(index2c, Deck.createCardIndex(Deck.RANK_2, Deck.SUIT_CLUBS));
    assertEquals(indexAs, Deck.createCardIndex(Deck.RANK_ACE, Deck.SUIT_SPADES));
    assertEquals(indexAh, Deck.createCardIndex(Deck.RANK_ACE, Deck.SUIT_HEARTS));
    assertEquals(indexAd, Deck.createCardIndex(Deck.RANK_ACE, Deck.SUIT_DIAMONDS));
    assertEquals(indexAc, Deck.createCardIndex(Deck.RANK_ACE, Deck.SUIT_CLUBS));

    // should match macro JokerDeck_JOKER in deck_joker.h
    assertEquals(indexJoker, Deck.createCardIndex(Deck.RANK_JOKER, Deck.SUIT_JOKER));

    // check illegal cases
    try {
      Deck.createCardIndex(Deck.RANK_COUNT, Deck.SUIT_SPADES);
      fail("Should raise IllegalArgumentException");
    } catch (RuntimeException e) {
    }
    try {
      Deck.createCardIndex(Deck.RANK_2, Deck.SUIT_COUNT);
      fail("Should raise IllegalArgumentException");
    } catch (RuntimeException e) {
    }
    try {
      Deck.createCardIndex(Deck.RANK_JOKER, Deck.SUIT_SPADES);
      fail("Should raise IllegalArgumentException");
    } catch (RuntimeException e) {
    }
    try {
      Deck.createCardIndex(Deck.RANK_2, Deck.SUIT_JOKER);
      fail("Should raise IllegalArgumentException");
    } catch (RuntimeException e) {
    }
  }

  public void testCreateCardMask() {
    // should match macro StdDeck_MASK in deck_std.h
    assertEquals(maskAs, Deck.createCardMask(Deck.RANK_ACE, Deck.SUIT_SPADES));
    // should match macro JokerDeck_MASK in deck_joker.h
    assertEquals(maskJoker,
                 Deck.createCardMask(Deck.RANK_JOKER, Deck.SUIT_JOKER));
    // create hand with "As Ah 2h"
    int[] ranks = {Deck.RANK_ACE, Deck.RANK_ACE, Deck.RANK_2};
    int[] suits = {Deck.SUIT_SPADES, Deck.SUIT_HEARTS, Deck.SUIT_HEARTS};
    long mask = Deck.createCardMask(ranks, suits);
    long expected = maskAs | maskAh | mask2h;
    assertEquals(expected, mask);
  }
  
  public void testParseCardMask() {
    long mask = Deck.parseCardMask("As Ah 2h");
    long expected = maskAs | maskAh | mask2h;
    assertEquals(expected, mask);
  }

  public void testCardString() {
    assertEquals("Td", Deck.cardString(Deck.RANK_TEN, Deck.SUIT_DIAMONDS));
    assertEquals("Xx", Deck.cardString(Deck.RANK_JOKER, Deck.SUIT_JOKER));
  }
  
  public void testParseRank() {
    assertEquals(Deck.RANK_ACE, Deck.parseRank("A"));
    assertEquals(Deck.RANK_TEN, Deck.parseRank("T"));
    assertEquals(Deck.RANK_2, Deck.parseRank("2"));
    assertEquals(Deck.RANK_JOKER, Deck.parseRank("X"));
  }
  
  public void testParseSuit() {
    assertEquals(Deck.SUIT_SPADES, Deck.parseSuit("s"));
    assertEquals(Deck.SUIT_HEARTS, Deck.parseSuit("h"));
    assertEquals(Deck.SUIT_DIAMONDS, Deck.parseSuit("d"));
    assertEquals(Deck.SUIT_CLUBS, Deck.parseSuit("c"));
  }

  public void testCardMaskString() {
    long mask = maskAs | maskAh | mask2h;
    assertEquals("As Ah 2h", Deck.cardMaskString(mask));
    assertEquals("AsAh2h", Deck.cardMaskString(mask, ""));
    assertEquals("As Ah 2h", Deck.cardMaskString(mask, " "));
    assertEquals("As/Ah/2h", Deck.cardMaskString(mask, "/"));
  }
}
