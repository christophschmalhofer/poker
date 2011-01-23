package handevaluator.generator.eval;

import handevaluator.generator.CardMultiSet;
import handevaluator.generator.EvaluatorChain;

public class ThreeOfAKindEvaluator extends EvaluatorChain {

	public ThreeOfAKindEvaluator(EvaluatorChain next) {
		super(next);
	}


	private String findKickerOne(int triplet, CardMultiSet set) {
		for (int i=0; i < set.getCardCount(); i++) {
			int c = set.getCard(i);
			if (c != triplet) {
				return findKickerTwo(triplet, c, set);
			}
		}		
		return null;
	}
	
	private String findKickerTwo(int triplet, int first, CardMultiSet set) {
		for (int i=0; i < set.getCardCount(); i++) {
			int c = set.getCard(i);
			if (c != triplet && c != first) {
				return buildCards(triplet, triplet, triplet, first, c);
			}
		}		
		return null;
	}

	@Override
	protected String calculateTopCards(CardMultiSet set) {
		// three passes: find two pairs, then find the kicker
		int running = 0, last = -1;
		for (int i=0; i < set.getCardCount(); i++) {
			if (set.getCard(i) == last) {
				running++;
				if (running == 2) {	// found triplet
					return findKickerOne(last, set);
				}
			} else {
				running = 0;
				last = set.getCard(i);
			}
		}
		return null;
	}
}
