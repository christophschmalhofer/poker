package handevaluator.purejava;

import handevaluator.classes.EquivalenceClass;
import handevaluator.classes.EquivalenceClasses;

import java.util.StringTokenizer;

public class DAGNode {
	private int id;
	private int[] prenext;
	protected DAGNode[] next;
	private int preflushclass, prerankclass;
	protected EquivalenceClass flushclass, rankclass;
	private String cards;

	public DAGNode(String l) {
		StringTokenizer t = new StringTokenizer(l, "\t");
		this.id = Integer.parseInt(t.nextToken());
		this.prenext = new int[13];
		for (int i=0; i < 13; i++) {
			prenext[i] = Integer.parseInt(t.nextToken());
		}
		preflushclass = Integer.parseInt(t.nextToken());
		prerankclass = Integer.parseInt(t.nextToken());
		cards = t.nextToken();
	}
	
	protected void wire(DAGNode[] nodes, EquivalenceClasses classes) {
		next = new DAGNode[13];
		for (int i=0; i < 13; i++) {
			if (prenext[i] > 0) {
				next[i] = nodes[prenext[i]];
			}
		}
		if (preflushclass > 0) {
			flushclass = classes.getEquivalenceClass(preflushclass);
		}
		if (prerankclass > 0) {
			rankclass = classes.getEquivalenceClass(prerankclass);
		}
	}

	public int getId() {
		return id;
	}

	public EquivalenceClass getFlushClass() {
		return flushclass;
	}

	public EquivalenceClass getRankClass() {
		return rankclass;
	}

	public String getCards() {
		return cards;
	}

}
