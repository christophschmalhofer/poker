package handevaluator.generator.eval;

import handevaluator.generator.CardMultiSet;
import handevaluator.generator.EvaluatorChain;

public class FullHouseEvaluator extends EvaluatorChain {
	public FullHouseEvaluator(EvaluatorChain next) {
		super(next);
	}

	private String findPair(int triple, CardMultiSet set) {
		int last = -1;
		for (int i=0; i < set.getCardCount(); i++) {
			if (set.getCard(i) == last) {
				return buildCards(triple, triple, triple, last, last);
			} else {
				if (set.getCard(i) != triple) {
					last = set.getCard(i);
				} else {
					last = -1;
				}
			}
		}
		return null;
	}

	@Override
	protected String calculateTopCards(CardMultiSet set) {
		// two passes: find a triplet, then a pair. Since the cards are ordered, there
		// is no problem with two triplets: the higher one is found before the lower one
		int running = 0, last = -1;
		for (int i=0; i < set.getCardCount(); i++) {
			if (set.getCard(i) == last) {
				running++;
				if (running == 2) {	// found triple
					return findPair(last, set);
				}
			} else {
				running = 0;
				last = set.getCard(i);
			}
		}
		return null;
	}
}
