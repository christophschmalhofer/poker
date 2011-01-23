package handevaluator.purejava;

import handevaluator.HandEvaluator;
import handevaluator.classes.EquivalenceClasses;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public abstract class PureJavaHandEvaluator extends HandEvaluator {
	protected DAGNode[] nodes;
	
	public PureJavaHandEvaluator(File dagfile) throws IOException {
		nodes = new DAGNode[76154];		
		BufferedReader r = new BufferedReader(new FileReader(dagfile));
		String l;
		int line = 0;
		while ((l = r.readLine()) != null) {
			nodes[line++] = new DAGNode(l);
		}
		System.out.println("Loaded " + line + " nodes");
		r.close();
		for (DAGNode n : nodes) {
			n.wire(nodes, EquivalenceClasses.getInstance());
		}
	}
}
