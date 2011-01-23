// $Id: HoldemAtomicGroup.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum;
import org.pokersource.game.Deck;

import java.util.HashSet;
import org.apache.oro.text.regex.MalformedPatternException;
import org.apache.oro.text.regex.MatchResult;
import org.apache.oro.text.regex.MatchResult;
import org.apache.oro.text.regex.Pattern;
import org.apache.oro.text.regex.Perl5Compiler;
import org.apache.oro.text.regex.Perl5Matcher;

/** A holdem hand group representing one exact starting hand such as
    "AhKh".
    @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemAtomicGroup extends HoldemHandGroup {
  /** Convert specific starting hand to HoldemAtomicGroup object.
      @param groupSpec starting hand (e.g., AhKd, 8h3s)
  */
  public HoldemAtomicGroup(String groupSpec) {
    myspec = groupSpec;
    myhands = new HashSet();
    Perl5Compiler compiler = new Perl5Compiler();
    Perl5Matcher matcher = new Perl5Matcher();
    Pattern atomicPattern;
    try {
      atomicPattern = compiler.compile
        ("^([AKQJT98765432])([shdc])([AKQJT98765432])([shdc])$");
    } catch (MalformedPatternException e) {
      throw new RuntimeException("BUG: " + e.toString());
    }
    MatchResult result;
    if (matcher.matches(groupSpec, atomicPattern)) {
      result = matcher.getMatch();
      int rank1 = Deck.parseRank(result.group(1));
      int suit1 = Deck.parseSuit(result.group(2));
      int rank2 = Deck.parseRank(result.group(3));
      int suit2 = Deck.parseSuit(result.group(4));
      addAtomic(rank1, suit1, rank2, suit2);
    } else {
      throw new IllegalArgumentException("unable to parse groupSpec: " +
                                         groupSpec);
    }
  }

  private void addAtomic(int rank1, int suit1, int rank2, int suit2) {
    if (rank1 == rank2 && suit1 == suit2)
      throw new IllegalArgumentException("atomic cards are identical");
    long card1 = Deck.createCardMask(rank1, suit1);
    long card2 = Deck.createCardMask(rank2, suit2);
    long hand = card1 | card2;
    myhands.add(new Long(hand));
  }

  public static void main(String[] args) {
    String groupSpec = args[0];
    HoldemAtomicGroup g = new HoldemAtomicGroup(groupSpec);
    System.out.println("spec=" + groupSpec + ", parsed=" + g.toString() +
                       ", atomic=" + g.toStringAtomic());
  }
}
