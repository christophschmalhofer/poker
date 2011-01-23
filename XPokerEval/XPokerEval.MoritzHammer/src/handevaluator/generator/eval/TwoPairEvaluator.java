package handevaluator.generator.eval;

import handevaluator.generator.CardMultiSet;
import handevaluator.generator.EvaluatorChain;

public class TwoPairEvaluator extends EvaluatorChain {
	public TwoPairEvaluator(EvaluatorChain next) {
		super(next);
	}

	private String findPair(int pair, CardMultiSet set) {
		int last = -1;
		for (int i=0; i < set.getCardCount(); i++) {
			if (set.getCard(i) == last) {
				return findKicker(pair, last, set);
			} else {
				if (set.getCard(i) != pair) {
					last = set.getCard(i);
				} else {
					last = -1;
				}
			}
		}
		return null;
	}

	private String findKicker(int pair, int last, CardMultiSet set) {
		for (int i=0; i < set.getCardCount(); i++) {
			int c = set.getCard(i);
			if (c != pair && c != last) {
				return buildCards(pair, pair, last, last, c);
			}
		}		
		return null;
	}

	@Override
	protected String calculateTopCards(CardMultiSet set) {
		// three passes: find two pairs, then find the kicker
		int last = -1;
		for (int i=0; i < set.getCardCount(); i++) {
			if (set.getCard(i) == last) {
				return findPair(last, set);
			} else {
				last = set.getCard(i);
			}
		}
		return null;
	}

}
