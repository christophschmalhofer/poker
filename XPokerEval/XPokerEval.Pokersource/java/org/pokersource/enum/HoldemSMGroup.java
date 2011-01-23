// $Id: HoldemSMGroup.java,v 1.3 2002/06/13 03:04:56 mjmaurer Exp $

package org.pokersource.enum;
import org.pokersource.game.*;
import java.util.HashSet;
import java.util.StringTokenizer;

/** A holdem hand group representing sets of starting hands corresponding
    to Sklansky & Malmuth groups.  "SM1" is group 1, etc., with "SM9"
    defined to be all hands not included in groups 1-8.
    @author Michael Maurer <mjmaurer@yahoo.com>
*/

public class HoldemSMGroup extends HoldemHandGroup {
  private static String[] smdefs = {
    // group 1
    "AA KK QQ JJ AKs",

    // group 2
    "TT AQs AJs KQs AK",

    // group 3
    "99 JTs QJs KJs ATs AQ",

    // group 4
    "T9s KQ 88 QTs 98s J9s AJ KTs",

    // group 5
    "77 87s Q9s T8s KJ QJ JT 76s 97s A9s A8s A7s A6s A5s A4s A3s A2s 65s",

    // group 6
    "66 AT 55 86s KT QT 54s K9s J8s",

    // group 7
    "44 J9 43s 75s T9 33 98 64s 22 K8s K7s K6s K5s K4s K3s K2s T7s Q8s",

    // group 8
    "87 53s A9 Q9 76 42s 32s 96s 85s J8 J7s 65 54 74s K9 T8",

    // group 9
    ("A8 A7 A6 A5 A4 A3 A2" +
     " K8 K7 K6 K5 K4 K3 K2" +
     " Q7s Q6s Q5s Q4s Q3s Q2s Q8 Q7 Q6 Q5 Q4 Q3 Q2" +
     " J6s J5s J4s J3s J2s J7 J6 J5 J4 J3 J2" +
     " T6s T5s T4s T3s T2s T7 T6 T5 T4 T3 T2" +
     " 95s 94s 93s 92s 97 96 95 94 93 92" +
     " 84s 83s 82s 86 85 84 83 82" +
     " 73s 72s 75 74 73 72" +
     " 63s 62s 64 63 62" +
     " 52s 53 52" +
     " 43 42" +
     " 32")
  };

  private int mygroup;

  public HoldemSMGroup(String groupSpec) {
    char c0 = groupSpec.charAt(0);
    char c1 = groupSpec.charAt(1);
    char c2 = groupSpec.charAt(2);
    if (groupSpec.length() != 3 ||c0 != 'S' || c1 != 'M' ||
        !Character.isDigit(c2))
      throw new IllegalArgumentException("invalid groupSpec: " + groupSpec);
    myspec = groupSpec;
    mygroup = Integer.valueOf(groupSpec.substring(2,3)).intValue();
    String smgroups = smdefs[mygroup - 1];
    StringTokenizer st = new StringTokenizer(smgroups);
    myhands = new HashSet();
    while (st.hasMoreTokens()) {
      String canonstr = st.nextToken();
      HoldemCanonGroup canon = new HoldemCanonGroup(canonstr);
      myhands.addAll(canon.myhands);
    }
  }

  public static void main(String[] args) {
    String groupSpec = args[0];
    HoldemSMGroup g = new HoldemSMGroup(groupSpec);
    System.out.println("spec=" + groupSpec + ", parsed=" + g.toString() +
                       ", atomic=" + g.toStringAtomic());
  }
}
