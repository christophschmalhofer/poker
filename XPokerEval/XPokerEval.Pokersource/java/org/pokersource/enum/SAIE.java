// $Id: SAIE.java,v 1.7 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum;
import org.pokersource.game.Deck;
import java.util.Map;
import java.util.Iterator;
import java.util.Enumeration;

/** Algorithms for computing subjective all-in equity.  SAIE is a player's pot
    equity given particular beliefs about the possible hands of the
    opponent(s) and assuming no further betting.  Beliefs about an opponent's
    hand distribution are represented by a BeliefVector object which maps each
    possible opponent hand to the probability of its occurrence.
    @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class SAIE {

  /** Compute the subjective all-in equity of each player based on a
      belief distribution for each player's hands.  Typical usage is
      to fix one player's cards and allow the other players' cards to
      range over a distribution; however, it is valid for all players to
      have multiple possible hands.
      @param gameType One of Enumerate.GAME_HOLDEM, etc.
      @param nmatchups number of matchups to sample (if zero, full enumeration)
      [Note: matchups are counted before they are tested for feasibility, that
      is, whether they share cards.  So the total number of matchups that
      contribute to the SAIE estimate may be less than nmatchups.]
      @param noutcomes number of boards to sample (if zero, full enumeration)
      @param handDistribs the hand distribution belief vector for each player
      @param board bitmask of cards already dealt to board (can be zero)
      @param dead bitmask of cards that cannot appear in any hand or on
      the board (can be zero)
      @param ev output: ev[i] player i's all-in pot equity
      @param matchups output: map of {HandMatchup, MatchupOutcome}
      pairs, one for each matchup
  */
  public static void FlopGameSAIE(int gameType,
                                  int nmatchups, int noutcomes,
                                  BeliefVector[] handDistribs,
                                  long board, long dead,
                                  double ev[], Map matchups) {
    if (matchups != null)
      matchups.clear();
    int nplayers = handDistribs.length;
    long[][] hands = new long[nplayers][];
    int[] nhands = new int[nplayers];
    for (int i=0; i<nplayers; i++) {
      hands[i] = handDistribs[i].getHands();
      nhands[i] = hands[i].length;
      ev[i] = 0;
    }
    long unavail1 = dead | board;
    double totalprob = 0;
    double[] matchev = new double[nplayers];
    long[] curhands = new long[nplayers];
    Enumeration enum;
    if (nmatchups == 0) {
      enum = new NestedLoopEnumeration(nhands);
    } else {
      enum = new NestedLoopSampling(nhands, nmatchups);
    }
  mainloop:
    while (enum.hasMoreElements()) { // loop over all hand matchups
      int[] indices = (int[]) enum.nextElement();
      long unavail2 = unavail1;
      double matchprob = 1;     // the probability of this matchup
      for (int i=0; i<nplayers; i++) {
        // get a particular hand for each player to use in this matchup
        curhands[i] = hands[i][indices[i]];
        if ((curhands[i] & unavail2) != 0) // already used one of these cards?
          continue mainloop;    	   // if so, this matchup cannot occur
        unavail2 |= curhands[i];
        matchprob *= handDistribs[i].getBeliefProb(curhands[i]);
        if (matchprob == 0)
          continue mainloop;
      }

      // heavy lifting for this matchup: enumerate all outcomes
      Enumerate.PotEquity(gameType, noutcomes, curhands, board, dead, matchev);

      if (matchups != null) { // save to Collection if requested
        HandMatchup matchup = new HandMatchup(curhands);
        MatchupOutcome outcome = new MatchupOutcome(matchprob, matchev);
        MatchupOutcome existing = (MatchupOutcome) matchups.get(matchup);
        if (existing != null)
          existing.merge(outcome);
        else
          matchups.put(matchup, outcome);
      }

      // accumulate this matchup into totals
      for (int i=0; i<nplayers; i++)
        ev[i] += matchev[i] * matchprob;
      totalprob += matchprob;
    }
    if (totalprob == 0)
      throw new IllegalArgumentException("no matchups sampled: increase nmatchups?");
    // Scale by the total probability of all matchups (this factor is less
    // than one when the hand distributions are not disjoint).
    for (int i=0; i<nplayers; i++)
      ev[i] /= totalprob;
    if (matchups != null) {
      for (Iterator iter=matchups.values().iterator(); iter.hasNext(); ) {
        MatchupOutcome outcome = (MatchupOutcome) iter.next();
        outcome.matchProb /= totalprob;
      }
    }
  }

  public static void main(String[] args) {
    int nmatchups = Integer.parseInt(args[0]);
    int noutcomes = Integer.parseInt(args[1]);
    int nplayers = args.length - 4;
    System.out.println("nplayers=" + nplayers + 
                       ", nmatchups=" + nmatchups +
                       ((nmatchups == 0) ? " (enumerate)" : " (sample)") +
                       ", noutcomes=" + noutcomes +
                       ((noutcomes == 0) ? " (enumerate)" : " (sample)"));
    HoldemBeliefVector[] beliefs = new HoldemBeliefVector[nplayers];
    for (int i=0; i<nplayers; i++) {
      beliefs[i] = new HoldemBeliefVector(args[i+2]);
      System.out.println("beliefs[" + i + "].toString = " +
                         beliefs[i].toString());
      System.out.println("beliefs[" + i + "].toStringAtomic = " +
                         beliefs[i].toStringAtomic());
    }
    long board = Deck.parseCardMask(args[args.length-2]);
    long dead = Deck.parseCardMask(args[args.length-1]);
    System.out.println("board = " + Deck.cardMaskString(board));
    System.out.println("dead = " + Deck.cardMaskString(dead));

    double[] totalev = new double[nplayers];
    FlopGameSAIE(Enumerate.GAME_HOLDEM, nmatchups, noutcomes,
                 beliefs, board, dead, totalev, null);
    for (int i=0; i<nplayers; i++) {
      System.out.println("FlopGameSAIE: totalev[" + i + "] = " + totalev[i]);
    }
  }
}
