package handevaluator.purejava;

import java.io.File;
import java.io.IOException;

import handevaluator.classes.EquivalenceClass;

public class TwoStagePJHE extends PureJavaHandEvaluator {
	public TwoStagePJHE(File dagfile) throws IOException {
		super(dagfile);
	}

	public EquivalenceClass calculateEquivalenceClass(byte[] hand) {
		byte[] color = new byte[]{0, 0, 0, 0};
		boolean flush = false;
		byte fc = 0;
		DAGNode n = null;;
		for (byte b : hand) {
			if (++color[b & 3] >= 5) {
				flush = true;
				fc = (byte)(b & 3);
			}			
			if (n == null) {
				n = nodes[b >> 2];
			} else {
				n = n.next[b >> 2];
			}
		}
		if (!flush) {
			return n.rankclass;
		}
		n = null;
		for (byte b : hand) {
			if ((b & 3) == fc) {
				if (n == null) {
					n = nodes[b >> 2];
				} else {
					n = n.next[b >> 2];
				}
			}
		}
		return n.flushclass;		
	}
}
