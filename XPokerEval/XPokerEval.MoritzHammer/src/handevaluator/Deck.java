package handevaluator;

public class Deck {
	private byte[] deck;
	private int dealt;
	public static final char[] RANKS = new char[]{'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
	public static final char[] COLORS = new char[]{'H', 'S', 'C', 'D'};	// arbitrary order

	public Deck() {
		deck = new byte[52];
		for (int i=0; i < 52; i++) {
			deck[i] = (byte) i;
		}
		dealt = 0;
	}
	
	public byte[] getBytes() {
		byte[] ret = new byte[dealt];
		System.arraycopy(deck, 0, ret, 0, dealt);
		return ret;
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("[");
		String delim = "";
		for (int i=0; i < dealt; i++) {
			sb.append(delim).append(RANKS[deck[i] >> 2]).append(COLORS[deck[i] & 3]);
			delim = " ";
		}
		return sb.append("]").toString();
	}
	
	public void randomHand() {
		randomHand(7);
	}
	
	public void randomHand(int cards) {		
		for (int i=0; i < cards; i++) {
			int idx = (int)(Math.random() * (double)(52 - i)) + i;
			byte swap = deck[i];
			deck[i] = deck[idx];
			deck[idx] = swap;
		}
		dealt = 7;
	}
}
