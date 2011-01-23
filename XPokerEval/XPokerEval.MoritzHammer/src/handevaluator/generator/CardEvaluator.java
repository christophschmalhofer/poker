package handevaluator.generator;


import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashSet;

public class CardEvaluator {
	private CardMultiSet[] layer;
	private int cards;
	private int depth;
	private CardMultiSet head;
	private int nodecount;

	public CardEvaluator(int depth, int cards, CardMultiSet[] initlayer, CardMultiSet head, int nodecount) {
		this.depth = depth;
		this.cards = cards;
		this.layer = initlayer;
		this.head = head;
		this.nodecount = nodecount;
	}

	public int getCards() {
		return cards;
	}

	public int getDepth() {
		return depth;
	}

	public CardMultiSet[] getLayer() {
		return layer;
	}

	public String toDOT() {
		StringBuilder ret = new StringBuilder();
		ret.append("digraph g {\n");
		ret.append("s[label=\"\", shape=none]\n");
		HashSet<CardMultiSet> set = new HashSet<CardMultiSet>();
		for (int i=0; i < cards; i++) {
			ret.append("s -> " + layer[i].getDOTName() + "\n");
			layer[i].toDOT(set, ret);
		}
		return ret.append("}\n").toString();
	}
	
	
	public void externalizeText(File f) throws FileNotFoundException {
		PrintWriter out = new PrintWriter(new FileOutputStream(f));
		
		CardMultiSet c = head;
		while (c != null) {
			StringBuilder b = new StringBuilder();
			b.append(c.getId());
			for (int i=0; i < 13; i++) {
				CardMultiSet cms = c.getLink(i);
				b.append("\t");
				if (cms == null) {
					b.append("0");
				} else {
					b.append(cms.getId());
				}				
			}
			b.append("\t").append(c.getECFlush());
			b.append("\t").append(c.getECNormal());
			b.append("\t").append(c.toString());
			b.append("\t").append(c.getMaxvalnormal());
			b.append("\t").append(c.getMaxvalflush());
			b.append("\t").append(c.getMinvalnormal());
			b.append("\t").append(c.getMinvalflush());
			c = c.getNext();
			out.println(b);		
		}
		out.close();
	}
	
	private void writeInt(int v, DataOutputStream out) throws IOException {
        out.write((v >>>  0) & 0xFF);
        out.write((v >>>  8) & 0xFF);
        out.write((v >>> 16) & 0xFF);
        out.write((v >>> 24) & 0xFF);
    }	
	
	public void externalize(File f) throws IOException {
		System.out.println("Externalizing to " + f);
		// this is cool: remove the BufferedOutputStream, and see the runtime... chaining works!
		DataOutputStream out = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(f)));
		// a small header: the number of nodes
		writeInt(nodecount, out);
		// each node consists of 
		// 32 bit: id (for consistency check purposes and padding)
		// 13*32 bit: successor id offset (i.e. the number of nodes to skip, 0 for invalid hands)
		// 32 bit: the flush value (0 for less than five card sequences)
		// 32 bit: the nonflush value (0 for non-seven card sequences)
		// total: 512 bits = 64 byte. No signing problems should occur, since the values all stay below 32k
		CardMultiSet c = head;		
		while (c != null) {
			writeInt(c.getId(), out);
			for (int i=0; i < 13; i++) {
				CardMultiSet cms = c.getLink(i);
				if (cms == null) {
					writeInt(0, out);
				} else {
					int offset = cms.getId() - c.getId();
					assert(offset > 0);
					writeInt(offset, out);
				}				
			}
			writeInt(Math.max(c.getECFlush(), 0), out);
			writeInt(Math.max(c.getECNormal(), 0), out);
			c = c.getNext();
		}
		out.close();
		System.out.println("Externalizing completed: wrote " + f.length() + " bytes, expecting " + (76154*64 + 4));
		assert(f.length() == 76154*64 + 4);
		assert(f.length() == nodecount*64 + 4);
	}
		
	public static void main(String[] args) throws IOException {
		CardEvaluator eval = HandEvalGenerator.buildCardSetDAG(7, 13);
		eval.externalizeText(new File("holdemclasses.graph"));
	}
}
