// $Id: Deck.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.game;

/**
 Some utilities for working with cards and cardmasks in a way that is
 consistent with all decks used by the poker-eval C library.  We use
 the joker deck because it is a superset of the other decks.
 @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class Deck {
  // must match values in deck_joker.h (and also deck_std.h, deck_astud.h)
  public static int SUIT_HEARTS		= 0;
  public static int SUIT_DIAMONDS	= 1;
  public static int SUIT_CLUBS		= 2;
  public static int SUIT_SPADES		= 3;
  public static int SUIT_COUNT		= 4;

  // must match values in deck_joker.h (and also deck_std.h, deck_astud.h)
  public static int RANK_2	= 0;
  public static int RANK_3	= 1;
  public static int RANK_4	= 2;
  public static int RANK_5	= 3;
  public static int RANK_6	= 4;
  public static int RANK_7	= 5;
  public static int RANK_8	= 6;
  public static int RANK_9	= 7;
  public static int RANK_TEN	= 8;
  public static int RANK_JACK	= 9;
  public static int RANK_QUEEN	= 10;
  public static int RANK_KING	= 11;
  public static int RANK_ACE	= 12;
  public static int RANK_COUNT	= 13;

  // this is a special value not found in deck_joker.h
  public static int RANK_JOKER	= 13;
  public static int SUIT_JOKER	= 4;

  /** Returns a number between 0 and 52 denoting the card index within
      the deck.  Consistent with all of StdDeck, JokerDeck, and AStudDeck
      in the poker-eval C library.
      @param rank The rank of the card
      @param suit The suit of the card
      @return index between 0 and 52
  */
  public static int createCardIndex(int rank, int suit) {
    if (rank >= 0 && rank < RANK_COUNT &&
        suit >= 0 && suit < SUIT_COUNT)		// 23456789TJQKA
      return (RANK_COUNT * suit + rank);	// 13 * suit + rank
    else if (rank == RANK_JOKER &&
             suit == SUIT_JOKER)		// joker
      return (RANK_COUNT * SUIT_COUNT);		// 52
    else
      throw new IllegalArgumentException("unknown rank and suit combination");
  }

  /** Returns a long integer with one bit set corresponding to the card
      index.  Consistent with all of StdDeck, JokerDeck, and AStudDeck
      in the poker-eval C library.
      @param rank The rank of the card
      @param suit The suit of the card
      @return bitmask with one bit set between bits 0 and 52
  */
  public static long createCardMask(int rank, int suit) {
    return (1L << createCardIndex(rank, suit));
  }

  /** Returns a long integer with one bit set for each card present in
      the ranks/suits arrays.  Consistent with all of StdDeck, JokerDeck,
      and AStudDeck in the poker-eval C library.
      @param ranks ranks[i] is the rank of the ith card
      @param suits suits[i] is the suit of the ith card
      @return bitmask of cards
  */
  public static long createCardMask(int[] ranks, int[] suits) {
    if (ranks.length != suits.length)
      throw new IllegalArgumentException("ranks and suits must be same length");
    long mask = 0;
    for (int i=0; i<ranks.length; i++)
      mask = mask | createCardMask(ranks[i], suits[i]);
    return mask;
  }

  private static String rankString = "23456789TJQKAX";	// matches deck_std.h
  private static String suitString = "hdcsx";		// matches deck_std.h

  /** Returns a long integer with one bit set for each card present in
      the input string.  Consistent with all of StdDeck, JokerDeck,
      and AStudDeck in the poker-eval C library.
      @param maskstr One or more cards, e.g., "Ac Td 2h" (whitespace optional)
      @return bitmask of cards
  */
  public static long parseCardMask(String maskstr) {
    int ncards = 0;
    long mask = 0;
    int pos = 0;
    while (pos < maskstr.length()) {
      while (pos < maskstr.length() && maskstr.charAt(pos) == ' ')
        pos++;
      if (pos < maskstr.length()) {
        int rank = parseRank(maskstr.substring(pos, pos+1));
        int suit = parseSuit(maskstr.substring(pos+1, pos+2));
        mask |= createCardMask(rank, suit);
        pos += 2;
      }
    }
    return mask;
  }

  /** Returns a string like "Ac", "Td", or "2h" representing the card.
      @param rank The rank of the card
      @param suit The suit of the card
      @return string representation of the card
  */
  public static String cardString(int rank, int suit) {
    if (!(rank >= 0 && rank < RANK_COUNT && suit >= 0 && suit < SUIT_COUNT) &&
        !(rank == RANK_JOKER && suit == SUIT_JOKER))
      throw new IllegalArgumentException("unknown rank and suit combination");
    if (rank == RANK_JOKER)
      suit = SUIT_JOKER;
    return (rankString.substring(rank, rank + 1) +
            suitString.substring(suit, suit + 1));
  }

  public static int parseRank(String rankstr) {
    int index = rankString.indexOf(rankstr);
    if (index < 0)
      throw new IllegalArgumentException("unable to parse rank: " + rankstr);
    return index;
  }

  public static int parseSuit(String suitstr) {
    int index = suitString.indexOf(suitstr);
    if (index < 0)
      throw new IllegalArgumentException("unable to parse suit: " + suitstr);
    return index;
  }

  public static String rankString(int rank) {
    return rankString.substring(rank, rank+1);
  }

  public static String suitString(int suit) {
    return suitString.substring(suit, suit+1);
  }

  /** Returns a string like "Ac Td 2h" representing the cards in mask.
      @param mask The bitmask of cards in the hand (from createCardMask())
      @param delim Delimiter to insert between each card (example above uses " ")
      @return string representation of the cards
  */
  public static String cardMaskString(long mask, String delim) {
    StringBuffer result = new StringBuffer();
    long m = createCardMask(RANK_JOKER, SUIT_JOKER);
    if ((mask & m) != 0)
      result.append(cardString(RANK_JOKER, SUIT_JOKER));
    for (int r=RANK_COUNT-1; r>=0; r--) {	
      for (int s=SUIT_COUNT-1; s>=0; s--) {
        m = createCardMask(r, s);
        if ((mask & m) != 0) {
          if (result.length() > 0)
            result.append(delim);
          result.append(cardString(r, s));
        }
      }
    }
    return result.toString();
  }
  /** Returns a string like "Ac Td 2h" representing the cards in mask.
      @param mask The bitmask of cards in the hand (from createCardMask())
      @return string representation of the cards
  */
  public static String cardMaskString(long mask) {
    return cardMaskString(mask, " ");
  }

}
