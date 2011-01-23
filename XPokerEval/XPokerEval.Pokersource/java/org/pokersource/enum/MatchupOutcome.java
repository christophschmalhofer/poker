// $Id: MatchupOutcome.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum;

/**
   @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class MatchupOutcome {
  public double matchProb;           // probability of this matchup
  public double matchEV[];           // matchEV[i] is player i's all-in equity

  public MatchupOutcome(double matchProb,
                        double matchEV[]) {
    this.matchProb = matchProb;
    this.matchEV = (double[]) matchEV.clone();
  }

  public void merge(MatchupOutcome other) {
    double oldWeight = this.matchProb / (this.matchProb + other.matchProb);
    double newWeight = other.matchProb / (this.matchProb + other.matchProb);
    for (int i=0; i<this.matchEV.length; i++) {
      this.matchEV[i] = (oldWeight * this.matchEV[i] +
                         newWeight * other.matchEV[i]);
    }
    this.matchProb += other.matchProb;
  }
}
