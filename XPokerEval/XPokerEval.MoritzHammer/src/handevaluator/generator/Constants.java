package handevaluator.generator;

import java.util.HashMap;
import java.util.Map;

public class Constants {
	public static final String[] CARD_NAMES = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
	public static final String[] COLOR_NAMES = {"H", "S", "C", "D"};
	public static final Map<Character,Integer> CARD_RANK = new HashMap<Character,Integer>(13);
	public static final Map<Character,Integer> CARD_COLOR = new HashMap<Character,Integer>(4);
	
	static {
		CARD_RANK.put('2', 0);
		CARD_RANK.put('3', 1);
		CARD_RANK.put('4', 2);
		CARD_RANK.put('5', 3);
		CARD_RANK.put('6', 4);
		CARD_RANK.put('7', 5);
		CARD_RANK.put('8', 6);
		CARD_RANK.put('9', 7);
		CARD_RANK.put('T', 8);
		CARD_RANK.put('J', 9);
		CARD_RANK.put('Q', 10);
		CARD_RANK.put('K', 11);
		CARD_RANK.put('A', 12);
		
		CARD_COLOR.put('H', 0);
		CARD_COLOR.put('S', 1);
		CARD_COLOR.put('C', 2);
		CARD_COLOR.put('D', 3);
	}
}
