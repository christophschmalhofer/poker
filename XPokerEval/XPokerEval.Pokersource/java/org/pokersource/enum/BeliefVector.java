// $Id: BeliefVector.java,v 1.5 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum;
import org.pokersource.game.Deck;
import java.util.HashSet;
import java.util.HashMap;
import java.util.Iterator;

/** Represents subjective beliefs about the possible hands held by a
    player.  Maintains a mapping from each hand to its probability of
    occurrence.
    @author Michael Maurer <mjmaurer@yahoo.com>
*/

public abstract class BeliefVector {
  /** Our string representation, from the constructor. */
  private String myspec;

  /** The universe of all possible hands.  Should be set in the subclass'
      constructor.  Used by addRemaining() to know which hands haven't
      yet been added. */
  HandGroup universalGroup;

  /** The belief probability (unconditioned by dead cards) of each hand group.
      Hash key is HoldemHandGroup, value is Double.  Positive value is the
      relative probability compared to uniform Bayesian prior (so, a value of
      +2 means hand from this group are twice as likely as would be expected
      from a uniform distribution over all possible hands); a negative value
      indicates an absolute probability (so -0.40 means that hands in this
      group account for 40% of the total probability).  Values must be either
      all positive or all negative. */
  private HashMap groupProb;

  /** The bitmask of dead cards.  This conditions the probabilities returned
      by getBeliefProb(). */
  private long deadCards;

  /** The belief probability (unconditioned by dead cards) for each atomic
      starting hand.  Updated as needed by computeUnconditionedHandProb().  */
  private HashMap uncondHandProb;

  /** The belief probability (conditioned by dead cards) for each atomic
      starting hand.  Updated as needed by computeConditionedHandProb().  */
  private HashMap condHandProb;

  private boolean hasRelative = false;
  private boolean hasAbsolute = false;

  /** Instantiate self from string respresentation.  Meant to be called from
      subclass's constructor.  The subclass constructor should then parse the
      string spec and populate groupProb and deadCards. */
  public BeliefVector(String spec) {
    myspec = spec;
    groupProb = new HashMap();
    deadCards = 0;
    uncondHandProb = null;
    condHandProb = null;
  }

  /** Instantiate self from string respresentation.  This method must be
      implemented by all subclasses.  Also, subclass constructors should call
      super(spec) and then fromString(spec). */
  public abstract void fromString(String spec);

  /** Generate string representation of self; the inverse of fromString(). */
  public String toString() {
    StringBuffer buf = new StringBuffer();
    for (Iterator iter = groupProb.keySet().iterator(); iter.hasNext(); ) {
      HandGroup group = (HandGroup) iter.next();
      if (buf.length() > 0)
        buf.append(" ");
      buf.append(group.toString());
      double prob = ((Double) groupProb.get(group)).doubleValue();
      String delim = (prob < 0) ? "=" : ":";
      int percent = (int) Math.round(100 * Math.abs(prob));
      buf.append(delim + percent);
    }
    if (deadCards != 0)
      buf.append(" / " + Deck.cardMaskString(deadCards));
    return buf.toString();
  }

  /** Generate a string representation of self that gives probability
      details for all atomic hands, conditioned on the dead cards. */
  public String toStringAtomic() {
    StringBuffer buf = new StringBuffer();
    for (Iterator iter = condHandProb.keySet().iterator(); iter.hasNext(); ) {
      Long lhand = (Long) iter.next();
      long hand = lhand.longValue();
      double prob = ((Double) condHandProb.get(lhand)).doubleValue();
      if (buf.length() > 1)
        buf.append(" ");
      buf.append(Deck.cardMaskString(hand, ""));
      int percent = (int) Math.round(10000 * Math.abs(prob));
      buf.append(":" + percent);
    }
    return buf.toString();
  }


  /** Return an array of bitmasks representing hands with nonzero probability
      of occurring (conditioned on the dead cards). */
  public long[] getHands() {
    long[] hands = new long[condHandProb.size()];
    int nhands = 0;
    for (Iterator iter = condHandProb.keySet().iterator(); iter.hasNext(); )
      hands[nhands++] = ((Long) iter.next()).longValue();
    return hands;
  }
  
  /** Return the absolute probability that hand will occur, conditioned on
      the dead cards. */
  public double getBeliefProb(long hand) {
    Double prob = (Double) condHandProb.get(new Long(hand));
    if (prob == null)
      return 0;
    else
      return prob.doubleValue();
  }

  private void computeConditionedHandProb() {
    // adjust hand probabilities by eliminating hands that require a dead card
    if (deadCards == 0) {
      condHandProb = uncondHandProb;
      return;
    }
    double deadProb = 0.0;	// total prob of hands containing dead cards
    for (Iterator iter = uncondHandProb.keySet().iterator(); iter.hasNext(); ) {
      Long key = (Long) iter.next();
      long hand = key.longValue();
      Double value = (Double) uncondHandProb.get(key);
      double hprob = value.doubleValue();
      if ((hand & deadCards) != 0) { // hand uses a dead card
        /*System.out.println("  condition: dead " + Deck.cardMaskString(hand) +
          " hprob=" + hprob);*/
        deadProb += hprob;
      }
    }
    condHandProb = new HashMap();
    if (deadProb > 0.999999)
      throw new IllegalArgumentException("dead cards exclude all hands");
    double condscale = 1 / (1 - deadProb);
    /*System.out.println("  condition: deadProb=" + deadProb + ", condscale="
      + condscale);*/
    for (Iterator iter = uncondHandProb.keySet().iterator(); iter.hasNext(); ) {
      Long key = (Long) iter.next();
      long hand = key.longValue();
      if ((hand & deadCards) == 0) { // hand does not use dead card
        Double value = (Double) uncondHandProb.get(key);
        double hprob = value.doubleValue();
        double condprob = hprob * condscale;
        /*System.out.println("  condition: update " + Deck.cardMaskString(hand) +
          " condprob=" + condprob);*/
        condHandProb.put(key, new Double(condprob));
      }
    }
  }

  private double totalRelativeProb() {
    // sum over groups of each group's relative prob
    // times the group's number of atomic hands
    double totalRel = 0.0;
    for (Iterator iter = groupProb.keySet().iterator(); iter.hasNext(); ) {
      HandGroup group = (HandGroup) iter.next();
      double gprob = ((Double) groupProb.get(group)).doubleValue();
      int gsize = group.getHands().length;
      if (gprob > 0)
        totalRel += gprob * gsize;
    }
    return totalRel;
  }

  private double totalAbsoluteProb() {
    // sum over groups of each group's absolte prob
    double totalAbs = 0.0;
    for (Iterator iter = groupProb.keySet().iterator(); iter.hasNext(); ) {
      HandGroup group = (HandGroup) iter.next();
      double gprob = ((Double) groupProb.get(group)).doubleValue();
      if (gprob < 0)
        totalAbs += -gprob;
    }
    return totalAbs;
  }

  private void computeUnconditionedHandProb() {
    uncondHandProb = new HashMap();
    double totalRel = totalRelativeProb();
    /*System.out.println("recomputing: totalRel=" + totalRel);*/
    for (Iterator iter = groupProb.keySet().iterator(); iter.hasNext(); ) {
      HandGroup group = (HandGroup) iter.next();
      long[] ghands = group.getHands();
      double gprob = ((Double) groupProb.get(group)).doubleValue();
      double hprob;   // absolute probability of each hand in this group
      if (gprob < 0) {           // group has absolute probability
        hprob = -gprob / ghands.length;
      } else if (gprob > 0) {    // group has relative probability
        hprob = gprob / totalRel;
      } else {
        hprob = 0;
      }
      /*System.out.println("           : group=" + group.toString() +
                         ", gsize=" + ghands.length +
                         ", gprob=" + gprob +
                         ", hprob=" + hprob);*/
      if (hprob > 0) {
        for (int i=0; i<ghands.length; i++) {
          Long key = new Long(ghands[i]);
          Double value = new Double(hprob);
          if (uncondHandProb.containsKey(key))
            throw new IllegalArgumentException
              ("duplicate hand: " + Deck.cardMaskString(ghands[i]));
          uncondHandProb.put(key, value);
        }
      }
    }
  }

 /** Set the "dead cards", cards that are known not to be available.  This
     sets the probability to zero of any hand including any of these cards
     increases the probabilities of the other hands in proportion. */
  public void setDeadCards(long cards) {
    /*System.out.println("DEAD " + Deck.cardMaskString(cards));*/
    deadCards = cards;
    computeConditionedHandProb();
  }

  private void addHandGroup(HandGroup group, double prob) {
    /*System.out.println("ADD group=" + group.toString() + ", prob=" + prob);*/
    groupProb.put(group, new Double(prob));
    computeUnconditionedHandProb();
    computeConditionedHandProb();
  }

  /** During construction, add a new hand group with its probability of
      occurrence relative to the Bayesian probability.  Meant to be called by
      subclass's fromString() */
  void addHandGroupRelative(HandGroup group, double relativeProb) {
    if (hasAbsolute)
      throw new IllegalArgumentException("cannot mix relative and absolute probs");
    addHandGroup(group, relativeProb);
    hasRelative = true;
  }
  
  /** During construction, add a new hand group with its absolute probability
      of occurrence.  The sum of absolute probabilities over groups should be
      1.  Meant to be called by subclass's fromString(). */
  void addHandGroupAbsolute(HandGroup group, double absoluteProb) {
    if (hasRelative)
      throw new IllegalArgumentException("cannot mix relative and absolute probs");
    addHandGroup(group, -absoluteProb);
    hasAbsolute = true;
  }

  private void addRemaining(double prob) {
    /*System.out.println("ADD all remaining hands, prob=" + prob);*/
    // Form a special hand group whose set of hands is the difference between
    // the universe of possible hands and hands present in groups we have
    // already added.
    HandGroup others = new HandGroup();
    others.myspec = "<other>";
    others.myhands = new HashSet();
    others.myhands.addAll(universalGroup.myhands);
    for (Iterator iter = groupProb.keySet().iterator(); iter.hasNext(); ) {
      HandGroup group = (HandGroup) iter.next();
      others.myhands.removeAll(group.myhands);
    }
    groupProb.put(others, new Double(prob));
    computeUnconditionedHandProb();
    computeConditionedHandProb();
  }

  /** During construction, add each hand not yet added with its probability of
      occurrence relative to its Bayesian probability.  Meant to be called by
      subclass's fromString() */
  void addRemainingRelative(double relativeProb) {
    if (hasAbsolute)
      throw new IllegalArgumentException("cannot mix relative and absolute probs");
    addRemaining(relativeProb);
    hasRelative = true;
  }
  
  /** During construction, add each hand not yet added with the given absolute
      probability of occurrence of the entire set of such hands.  The sum of
      absolute probabilities over groups should be 1.  Meant to be called by
      subclass's fromString(). */
  void addRemainingAbsolute(double absoluteProb) {
    if (hasRelative)
      throw new IllegalArgumentException("cannot mix relative and absolute probs");
    addRemaining(-absoluteProb);
    hasAbsolute = true;
  }

  /** After construction, subclasses should call this for a sanity check. */
  void validate() {
    if (hasAbsolute) {
      if (totalAbsoluteProb() != 1.0)
        throw new IllegalArgumentException("absolute probabilities must sum to 1");
    }
    if (hasRelative) {
      if (totalRelativeProb() <= 0)
        throw new IllegalArgumentException("relative probabilities must be positive");
    }
  }
}
