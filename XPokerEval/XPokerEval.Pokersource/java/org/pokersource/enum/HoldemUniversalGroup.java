// $Id: HoldemUniversalGroup.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum;
import org.pokersource.game.Deck;

import java.util.HashSet;

/** A holdem hand group representing every possible starting Holdem hand.
    @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemUniversalGroup extends HoldemHandGroup {
  /** Return a group containing all possible holdem hands
      @param groupSpec Must have value "<any>"
  */
  public HoldemUniversalGroup(String groupSpec) {
    if (!groupSpec.equals("<any>"))
      throw new IllegalArgumentException("unable to parse spec: " + groupSpec);
    myspec = groupSpec;
    myhands = new HashSet();
    for (int rank1=0; rank1<Deck.RANK_COUNT; rank1++) {
      for (int rank2=rank1; rank2<Deck.RANK_COUNT; rank2++) {
        for (int suit1=0; suit1<Deck.SUIT_COUNT; suit1++) {
          for (int suit2=0; suit2<Deck.SUIT_COUNT; suit2++) {
            if (rank1 == rank2 && suit1 >= suit2)
              continue;
            addAtomic(rank1, suit1, rank2, suit2);
          }
        }
      }
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
    String groupSpec = "<any>";
    HoldemUniversalGroup g = new HoldemUniversalGroup(groupSpec);
    System.out.println("spec=" + groupSpec + ", parsed=" + g.toString() +
                       ", atomic=" + g.toStringAtomic());
  }
}
