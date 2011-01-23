package handevaluator.generator;

public class EquivalenceClass {
	private int id;
	private String cards;
	private String group;
	private String type;

	public String toString() {
		return id + ": " + cards + " (" + group + ") " + type;
	}
	
	public EquivalenceClass(int id, String cards, String group, String type) {
		this.id = id;
		this.cards = cards;
		this.group = group;
		this.type = type;
	}

	public String getCards() {
		return cards;
	}

	public String getGroup() {
		return group;
	}

	public int getId() {
		return id;
	}

	public String getType() {
		return type;
	}
}
