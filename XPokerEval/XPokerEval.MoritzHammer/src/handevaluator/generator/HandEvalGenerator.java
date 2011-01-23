package handevaluator.generator;

import handevaluator.generator.eval.FourOfAKindEvaluator;
import handevaluator.generator.eval.FullHouseEvaluator;
import handevaluator.generator.eval.HighCardEvaluator;
import handevaluator.generator.eval.PairEvaluator;
import handevaluator.generator.eval.StraightEvaluator;
import handevaluator.generator.eval.ThreeOfAKindEvaluator;
import handevaluator.generator.eval.TwoPairEvaluator;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;

public class HandEvalGenerator {
	public static EquivalenceCalculator equivalencecalc;
	
	static{
		try {
			equivalencecalc = new EquivalenceCalculator(new File("equivalenceclasses"));
		} catch (IOException e) {
			System.err.println("Failed to load equivalence classes:" + e);
		}
	}
	
	private static EvaluatorChain chain = 
		new FourOfAKindEvaluator(
		new FullHouseEvaluator(
		new StraightEvaluator(
		new ThreeOfAKindEvaluator(
		new TwoPairEvaluator(
		new PairEvaluator(
		new HighCardEvaluator(null)
		)
		)
		)
		)
		)
		);

	/**
	 we apply a chain of evaluators to the card set. The evaluator implementation
	 are required to check if the hand is in the equivalence classes they represent,
	 and if so, calculate the number of the equivalence class. Otherwise, the 
	 next evaluator is to be queried.
	 Flushes are not considered, so the first evaluator checks Four of a Kind.
	 The equivalence classes we use are those of  http://www.suffecool.net/poker/7462.html
	 althought roughly 2/3 are not used. This does not help - for 7 cards, there
	 are 4,824 equivalence classes, for 5, there are 7,462, so nothing is gained wrt.
	 bit representation (it is possible, that this are not exactly all equivalence
	 classes, since the 5-4-3-2-A straight is neglected. I am not sure about this,
	 maybe there are two equivalence classes more (straight flush/straight)).
	*/
	public static void calculateEquivalenceClass(CardMultiSet set) {
		int nec = -1;
		int fec = -1;
		if (set.getCardCount() >= 5) {
			String topcards = chain.getTopCards(set);
			if (topcards == null) {
				throw new RuntimeException("Cannot find top cards for " + set);
			}
			fec = equivalencecalc.getEquivalenceClass(topcards, true);
			if (set.getCardCount() == 7) {
				nec = equivalencecalc.getEquivalenceClass(topcards, false);
				assert(nec > 0);
			}
		}
		set.setECFlush(fec);
		set.setECNormal(nec);
	}
	
	public static CardEvaluator buildCardSetDAG(int depth, int cards) {
		int nodeid = 0;
		CardMultiSet head = null;
		HashMap<CardMultiSet, CardMultiSet> map = new HashMap<CardMultiSet, CardMultiSet>();
		LinkedList<CardMultiSet> layer = new LinkedList<CardMultiSet>();
		CardMultiSet initlayer[] = new CardMultiSet[cards];
		for (int i=0; i < cards; i++) {
			CardMultiSet cms = new CardMultiSet(i, depth, cards);
			layer.add(cms);
			map.put(cms, cms);
			calculateEquivalenceClass(cms);	// actually resets both classes to -1
			initlayer[i] = cms;
			cms.setId(nodeid++);
			if (head != null) {
				head.setNext(cms);
			}
			head = cms;
		}
		for (int i=1; i < depth; i++) {
			LinkedList<CardMultiSet> newlayer = new LinkedList<CardMultiSet>();
			for (CardMultiSet cms : layer) {
				for (int j=0; j < cards; j++) {
					CardMultiSet ncms = cms.duplicateWithNewCard(j);
					if (ncms.isRealHand()) {
						if (map.containsKey(ncms)) {
							ncms = map.get(ncms);
						} else {
							newlayer.add(ncms);
							map.put(ncms, ncms);
							ncms.setId(nodeid++);
							calculateEquivalenceClass(ncms);	// actually resets both classes to -1
							head.setNext(ncms);
							head = ncms;
						}
						cms.addLink(j, ncms);
					}
				}
			}
			layer = newlayer;
		}
		CardEvaluator ret = new CardEvaluator(depth, cards, initlayer, initlayer[0], nodeid);
		assert(map.size() == nodeid);
		System.out.println("Allocated " + map.size() + " nodes");
		return ret;
	}
}
