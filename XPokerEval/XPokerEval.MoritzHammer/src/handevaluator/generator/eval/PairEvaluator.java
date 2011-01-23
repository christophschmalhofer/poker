package handevaluator.generator.eval;

import handevaluator.generator.CardMultiSet;
import handevaluator.generator.EvaluatorChain;

public class PairEvaluator extends EvaluatorChain {
	public PairEvaluator(EvaluatorChain next) {
		super(next);
	}

	private String findKickerOne(int pair, CardMultiSet set) {
		for (int i=0; i < set.getCardCount(); i++) {
			int c = set.getCard(i);
			if (c != pair) {
				return findKickerTwo(pair, c, set);
			}
		}		
		return null;
	}
	
	private String findKickerTwo(int pair, int first, CardMultiSet set) {
		for (int i=0; i < set.getCardCount(); i++) {
			int c = set.getCard(i);
			if (c != pair && c != first) {
				return findKickerThree(pair, first, c, set);
			}
		}		
		return null;
	}

	private String findKickerThree(int pair, int first, int second, CardMultiSet set) {
		for (int i=0; i < set.getCardCount(); i++) {
			int c = set.getCard(i);
			if (c != pair && c != first && c != second) {
				return buildCards(pair, pair, first, second, c);
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
				return findKickerOne(last, set);
			} else {
				last = set.getCard(i);
			}
		}
		return null;
	}
}
