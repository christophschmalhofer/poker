package handevaluator.generator.eval;

import handevaluator.generator.CardMultiSet;
import handevaluator.generator.EvaluatorChain;

public class HighCardEvaluator extends EvaluatorChain {
	public HighCardEvaluator(EvaluatorChain next) {
		super(next);
	}

	@Override
	protected String calculateTopCards(CardMultiSet set) {
		// ok, we now know that the hand is utterly uninteresting. Since the cards
		// are already sorted, we take the first five cards and calculate the equivalence
		// class, althought this is slightly awkward. 
		return buildCards(set.getCard(0), set.getCard(1), set.getCard(2), set.getCard(3), set.getCard(4));
	}

}
