package handevaluator.purejava;

import handevaluator.classes.EquivalenceClass;

import java.io.File;
import java.io.IOException;

public class OneStagePJHE extends PureJavaHandEvaluator {
	public OneStagePJHE(File dagfile) throws IOException {
		super(dagfile);
	}

	public EquivalenceClass calculateEquivalenceClass(byte[] hand) {
		DAGNode fn[] = new DAGNode[4];
		DAGNode rn = null;
		for (byte b : hand) {
			byte col = (byte)(b & 3);
			if (fn[col] == null) {
				fn[col] = nodes[b >> 2];
			} else {
				fn[col] = fn[col].next[b >> 2];
			}
			if (rn == null) {
				rn = nodes[b >> 2];
			} else {
				rn = rn.next[b >> 2];
			}
		}
		for (DAGNode f : fn) {
			if (f != null && f.flushclass != null) {
				return f.flushclass;
			}
		}
		return rn.rankclass;
	}
}

