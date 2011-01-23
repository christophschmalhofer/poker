// $Id: HoldemAbdulGroup.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum;
import org.pokersource.game.*;

import java.util.HashSet;
import org.apache.oro.text.regex.MalformedPatternException;
import org.apache.oro.text.regex.MatchResult;
import org.apache.oro.text.regex.MatchResult;
import org.apache.oro.text.regex.Pattern;
import org.apache.oro.text.regex.Perl5Compiler;
import org.apache.oro.text.regex.Perl5Matcher;

/** A holdem hand group representing sets of starting hands defined using
    abdulian notation.  The notation is:
<pre>
      99+	pair >= 99
      ATs+	suited ace with kicker >= T
      KTs+	suited king with kicker >= T
      QTs+	suited queen with kicker >= T
      9xs+	any suited hand with a nine or higher (except those above)
      76s+	suited 0-gap connector, JT-76
      75s+	suited 1-gap connector, J9-75
      74s+	suited 2-gap connector, J8-74
      AT+	offsuit ace with kicker >= T
      KT+	offsuit king with kicker >= T
      QT+	offsuit queen with kicker >= T
      9x+	any offsuit hand with a nine or higher (except those above)
      76+	offsuit 0-gap connector, JT-76
      75+	offsuit 1-gap connector, J9-75
      74+	offsuit 2-gap connector, J8-74
</pre>
    @author Michael Maurer <mjmaurer@yahoo.com>
*/


public class HoldemAbdulGroup extends HoldemHandGroup {

  // one glitch: the any rank hands (9xs+, 8x+) overlap with the others

  public HoldemAbdulGroup(String groupSpec) {
    myspec = groupSpec;
    myhands = new HashSet();
    Perl5Compiler compiler = new Perl5Compiler();
    Perl5Matcher matcher = new Perl5Matcher();
    Pattern pairPattern, acePattern, kingPattern, queenPattern, rankPattern,
      gap0Pattern, gap1Pattern, gap2Pattern;
    try {
      pairPattern = compiler.compile("^([AKQJT98765432])(\\1)\\+$");
      acePattern = compiler.compile("^(A)([KQJT98765432])(s?)\\+$");
      kingPattern = compiler.compile("^(K)([QJT98765432])(s?)\\+$");
      queenPattern = compiler.compile("^(Q)([JT98765432])(s?)\\+$");
      rankPattern = compiler.compile("^([AKQJT9876543])(x)(s?)\\+$");
      gap0Pattern = compiler.compile("^(JT|T9|98|87|76|65|54|43|32)(s?)\\+$");
      gap1Pattern = compiler.compile("^(J9|T8|97|86|75|64|53|42)(s?)\\+$");
      gap2Pattern = compiler.compile("^(J8|T7|96|85|74|63|52)(s?)\\+$");
    } catch (MalformedPatternException e) {
      throw new RuntimeException("BUG: " + e.toString());
    }
    if (matcher.matches(groupSpec, pairPattern)) {
      MatchResult result = matcher.getMatch();
      int rank = Deck.parseRank(result.group(1));
      for (int prank=rank; prank<=Deck.RANK_ACE; prank++) {
        String srank = Deck.rankString(prank);
        HoldemCanonGroup canon = new HoldemCanonGroup(srank + srank);
        myhands.addAll(canon.myhands);
      }
    } else if (matcher.matches(groupSpec, acePattern) ||
               matcher.matches(groupSpec, kingPattern) ||
               matcher.matches(groupSpec, queenPattern) ||
               matcher.matches(groupSpec, rankPattern)) { // try rankPattern last
      MatchResult result = matcher.getMatch();
      String srank1 = result.group(1);
      String srank2 = result.group(2);
      String ssuited = result.group(3);
      if (srank2.equals("x"))
        srank2 = "2";
      int rank1 = Deck.parseRank(srank1);
      int rank2 = Deck.parseRank(srank2);
      for (int prank=rank2; prank<rank1; prank++) {
        srank2 = Deck.rankString(prank);
        HoldemCanonGroup canon = new HoldemCanonGroup(srank1 + srank2 + ssuited);
        myhands.addAll(canon.myhands);
      }
    } else if (matcher.matches(groupSpec, gap0Pattern) ||
               matcher.matches(groupSpec, gap1Pattern) ||
               matcher.matches(groupSpec, gap2Pattern)) {
      MatchResult result = matcher.getMatch();
      String srank1 = result.group(1).substring(0, 1);
      String srank2 = result.group(1).substring(1, 2);
      String ssuited = result.group(2);
      int rank1 = Deck.parseRank(srank1);
      int rank2 = Deck.parseRank(srank2);
      int gap = rank1 - rank2;
      // gap-based Abdulian hands are all jack-high or lower
      for (int prank1=rank1; prank1<=Deck.RANK_JACK; prank1++) {
        int prank2 = prank1 - gap;
        srank1 = Deck.rankString(prank1);
        srank2 = Deck.rankString(prank2);
        HoldemCanonGroup canon = new HoldemCanonGroup(srank1 + srank2 + ssuited);
        myhands.addAll(canon.myhands);
      }
    } else {
      throw new IllegalArgumentException("unable to parse groupSpec: " +
                                         groupSpec);
    }
  }

  public static void main(String[] args) {
    String groupSpec = args[0];
    HoldemAbdulGroup g = new HoldemAbdulGroup(groupSpec);
    System.out.println("spec=" + groupSpec + ", parsed=" + g.toString() +
                       ", atomic=" + g.toStringAtomic());
  }
}
