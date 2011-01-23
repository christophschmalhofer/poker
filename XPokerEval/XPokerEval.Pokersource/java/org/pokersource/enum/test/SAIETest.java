// $Id: SAIETest.java,v 1.6 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum.test;
import org.pokersource.enum.*;
import org.pokersource.game.Deck;
import java.util.HashMap;
import java.util.Iterator;

import junit.framework.*;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class SAIETest extends TestCase {
  private HoldemBeliefVector bv1;
  private HoldemBeliefVector bv2;
  private HoldemBeliefVector bv3;
  private HoldemBeliefVector bv4;
  private HoldemBeliefVector bv5;
  private long mask5h4h3d;
  
  public SAIETest(String name) {
    super(name);
  }
  public static void main(String args[]) {
    junit.textui.TestRunner.run(SAIETest.class);
  }

  protected void setUp() {
    bv1 = new HoldemBeliefVector("KhQc");
    bv2 = new HoldemBeliefVector("SM1 SM2 SM3");
    bv3 = new HoldemBeliefVector("SM1 SM2 SM3 SM4 SM5");
    bv4 = new HoldemBeliefVector("TdTc");
    bv5 = new HoldemBeliefVector("65s");
    mask5h4h3d = Deck.parseCardMask("5h 4h 3d");
  }

  public void checkEV(double ev[], HashMap matchups) {
    double[] evm = new double[ev.length];
    double totalprob = 0.0;
    for (Iterator iter=matchups.values().iterator(); iter.hasNext(); ) {
      MatchupOutcome mo = (MatchupOutcome) iter.next();
      for (int i=0; i<ev.length; i++) {
        evm[i] += mo.matchEV[i] * mo.matchProb;
      }
      totalprob += mo.matchProb;
    }
    assertEquals(1.0, totalprob, 1e-10);
    for (int i=0; i<ev.length; i++) {
      evm[i] /= totalprob;
      assertEquals(evm[i], ev[i], 1e-10);
    }
  }

  public void testFlopGameSAIE() {
    double[] ev = new double[2];
    // KhQc vs "SM1 SM2 SM3" with board of 5h4h3d
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, 0, 0,
                      new BeliefVector[] {bv1, bv2},
                      mask5h4h3d, 0, ev, null);
    assertEquals(0.279634179634, ev[0], 1e-10);
    assertEquals(0.720365820366, ev[1], 1e-10);

    // KhQc vs "SM1 SM2 SM3 SM4 SM5" with board of 5h4h3d
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, 0, 0,
                      new BeliefVector[] {bv1, bv3},
                      mask5h4h3d, 0, ev, null);
    assertEquals(0.416602297485, ev[0], 1e-10);
    assertEquals(0.583397702515, ev[1], 1e-10);
  }

  public void testFlopGameSAIE_Matchup() {
    double[] ev = new double[2];
    HashMap matchups = new HashMap();
    // KhQc vs TdTc with board of 5h4h3d
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, 0, 0,
                      new BeliefVector[] {bv1, bv4},
                      mask5h4h3d, 0, ev, matchups);
    assertEquals(0.298484848485, ev[0], 1e-10);
    assertEquals(0.701515151515, ev[1], 1e-10);
    assertEquals(1, matchups.size());
    checkEV(ev, matchups);

    // KhQc vs "SM1 SM2 SM3" with board of 5h4h3d
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, 0, 0,
                      new BeliefVector[] {bv1, bv2},
                      mask5h4h3d, 0, ev, matchups);
    assertEquals(0.279634179634, ev[0], 1e-10);
    assertEquals(0.720365820366, ev[1], 1e-10);
    assertEquals((6+3+3+6+3) + (6+3+4+2+9) + (6+4+3+3+4+9),
                 matchups.size());
    checkEV(ev, matchups);
  }

  public void testFlopGameSAIE_Sampling() {
    double[] ev = new double[2];
    HashMap matchups = new HashMap();
    int nmatchups;
    int noutcomes;

    // "65s" vs "SM1 SM2 SM3" with board of 5h4h3d
    // first full enumeration
    nmatchups = 0;
    noutcomes = 0;
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, nmatchups, noutcomes,
                      new BeliefVector[] {bv5, bv2},
                      mask5h4h3d, 0, ev, matchups);
    assertEquals(0.640960327917, ev[0], 1e-10);
    assertEquals(0.359039672083, ev[1], 1e-10);
    assertEquals(3 * ((6+6+6+6+4) + (6+4+4+4+12) + (6+4+4+4+4+12)),
                 matchups.size());
    checkEV(ev, matchups);

    nmatchups = 100;
    noutcomes = 0;
    // now sample matchups but use full enumeration for cards to come
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, nmatchups, noutcomes,
                      new BeliefVector[] {bv5, bv2},
                      mask5h4h3d, 0, ev, matchups);
    // use a tolerance that is large enough that it is unlikely to be exceeded
    // due to random chance, but small enough to catch programming blunders
    assertEquals(0.640960327917, ev[0], 7e-02);
    assertEquals(0.359039672083, ev[1], 7e-02);
    assertTrue(matchups.size() <= nmatchups);
    checkEV(ev, matchups);

    nmatchups = 0;
    noutcomes = 100;
    // now enumerate matchups but sample cards to come
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, nmatchups, noutcomes,
                      new BeliefVector[] {bv5, bv2},
                      mask5h4h3d, 0, ev, matchups);
    assertEquals(0.640960327917, ev[0], 7e-02);
    assertEquals(0.359039672083, ev[1], 7e-02);
    assertEquals(3 * ((6+6+6+6+4) + (6+4+4+4+12) + (6+4+4+4+4+12)),
                 matchups.size());
    checkEV(ev, matchups);

    nmatchups = 100;
    noutcomes = 100;
    // finally sample both matchups and cards to come
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, nmatchups, noutcomes,
                      new BeliefVector[] {bv5, bv2},
                      mask5h4h3d, 0, ev, matchups);
    assertEquals(0.640960327917, ev[0], 7e-02);
    assertEquals(0.359039672083, ev[1], 7e-02);
    assertTrue(matchups.size() <= nmatchups);
    checkEV(ev, matchups);
  }

  public void testFlopGameSAIE_Multiway() {
    double[] ev = new double[4];
    HashMap matchups = new HashMap();
    int nmatchups;
    int noutcomes;

    // KhQc vs "SM1 SM2 SM3" vs "SM1 SM2 SM3 SM4 SM5" vs "65s"
    // with board of 5h4h3d
    nmatchups = 1000;
    noutcomes = 0;
    SAIE.FlopGameSAIE(Enumerate.GAME_HOLDEM, nmatchups, noutcomes,
                      new BeliefVector[] {bv1, bv2, bv3, bv5},
                      mask5h4h3d, 0, ev, matchups);
    assertEquals(0.12, ev[0], 2e-02);
    assertEquals(0.20, ev[1], 2e-02);
    assertEquals(0.19, ev[2], 2e-02);
    assertEquals(0.49, ev[3], 5e-02);
    checkEV(ev, matchups);
  }

}

