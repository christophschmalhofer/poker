package handevaluator.generator.eval;

import handevaluator.generator.CardMultiSet;
import handevaluator.generator.EvaluatorChain;

public class StraightEvaluator extends EvaluatorChain {

	public StraightEvaluator(EvaluatorChain next) {
		super(next);
	}

	@Override
	protected String calculateTopCards(CardMultiSet set) {
		// this is hardest -- try and find five consecutive cards, but they may be 
		// interspersed by pairs... and an ace may be low, so we can just keep track of that one
		int last = -1, steps = 0, start = -1;
		boolean ace = false;
		for (int i=0; i < set.getCardCount(); i++) {
			int c = set.getCard(i);
			if (c == 12) {
				ace = true;
			}
			if (c == last - (steps + 1)) {	// ace may be low in the straight
				steps++;
				if (steps == 4) {					
					return buildCards(start, start-1, start-2, start-3, c);
				}
			} else if (c == last - steps) {
			} else {
				steps = 0;
				last = c;
				start = c;
			}
		}
		if (ace && steps == 3 && last == 3) {
			return buildCards(start, start-1, start-2, start-3, 12);	// special for ace low
		}
		return null;
	}
}
