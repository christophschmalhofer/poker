package handevaluator.generator;

public abstract class EvaluatorChain {
	private EvaluatorChain next;
	
	public EvaluatorChain(EvaluatorChain next) {
		this.next = next;
	}

	public String getTopCards(CardMultiSet set) {		
		String res = calculateTopCards(set);
		if (res != null) {
//			System.out.println("Matched by " + getClass().getName() + ": " + res);
			return res;
		}
		if (next != null) {
			return next.getTopCards(set);
		} else {
			throw new RuntimeException("Cannot find equivalence class for " + set);
		}
	}
	
	protected abstract String calculateTopCards(CardMultiSet set);
	
	protected String buildCards(int c1, int c2, int c3, int c4, int c5) {
		StringBuilder ret = new StringBuilder();
		ret.append(Constants.CARD_NAMES[c1]).append(" ");
		ret.append(Constants.CARD_NAMES[c2]).append(" ");
		ret.append(Constants.CARD_NAMES[c3]).append(" ");
		ret.append(Constants.CARD_NAMES[c4]).append(" ");
		ret.append(Constants.CARD_NAMES[c5]);
		return ret.toString();
	}

}
