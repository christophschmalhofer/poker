// $Id: HandGroup.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum;
import org.pokersource.game.Deck;
import java.util.HashSet;
import java.util.Iterator;

/** A HandGroup represents a set of poker hands that can be referred to by
    name.  The specific way that hands are assigned to named groups is
    specific to each poker game.  For example, in Holdem, named groups
    include specific holdings like "AhAd", "Kh2h"; canonical starting
    hands like "AKs", "TT"; or abdulian groups like "Q8s+".  Subclasses
    like HoldemHandGroup define these groups for specific games.
    @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HandGroup {
  /** String representation of hand group.  Subclasses should accept this
      string in the constructor and save it here. */
  String myspec;

  /** Set of Long objects, each a bitmask for one hand.  Subclasses should,
      in their constructor, convert myspec into the set of corresponding
      hands.  The set should be immutable once set in the constructor. */
  HashSet myhands;

  // subclasses should have constructor of form: <init>(String groupSpec);

  public boolean isHandInGroup(long hand) {
    return myhands.contains(new Long(hand));
  }

  public long[] getHands() {
    long[] hands = new long[myhands.size()];
    int nhands = 0;
    for (Iterator i = myhands.iterator(); i.hasNext(); )
      hands[nhands++] = ((Long) i.next()).longValue();
    return hands;
  }

  public String toString() {
    return myspec;
  }

  public String toStringAtomic() {
    StringBuffer buf = new StringBuffer();
    for (Iterator i = myhands.iterator(); i.hasNext(); ) {
      long hand = ((Long) i.next()).longValue();
      if (buf.length() > 1)
        buf.append(" ");
      buf.append(Deck.cardMaskString(hand, ""));
    }
    return buf.toString();
  }
}
