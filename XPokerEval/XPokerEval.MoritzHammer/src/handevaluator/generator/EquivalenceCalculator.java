package handevaluator.generator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class EquivalenceCalculator {
	private Map<String, EquivalenceClass> eqcl;
	private ArrayList<EquivalenceClass> classes;
	
	public EquivalenceCalculator(File file) throws IOException {
		Pattern p = Pattern.compile("^\\s*(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+" +
				"(\\w\\s\\w\\s\\w\\s\\w\\s\\w)\\s+(\\w+)\\s+(.*)");
		BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
		String line;
		int linec = 0;
		eqcl = new HashMap<String, EquivalenceClass>();
		classes = new ArrayList<EquivalenceClass>();
		while ((line = r.readLine()) != null) {
			linec++;
			Matcher m = p.matcher(line);
			if (!m.matches()) {
				throw new IOException("Cannot parse line " + linec + ": " + line);
			}
			int id = Integer.parseInt(m.group(1));
			String cards = m.group(6);
			String group = m.group(7);
			String type = m.group(8);
			EquivalenceClass ec = new EquivalenceClass(id, cards, group, type);
			if (!group.equals("F") && !group.equals("SF")) {	
				eqcl.put("N " + cards, ec);
			} else {
				eqcl.put("F " + cards, ec);
			}	
			classes.add(ec);
		}
		r.close();
	}

	/** Get the equivalence class. Some equivalence classes do not exist, like
	 * pair cards for flush hands. In this case, -1 is returned. 
	 * @param topcards the string of topcards
	 * @param flush true if this is to be a flush hand
	 * @return the id of the equivalence class (starting with 1), or -1 if it does not exist
	 */
	public int getEquivalenceClass(String topcards, boolean flush) {
		EquivalenceClass c = eqcl.get((flush?"F ": "N ") + topcards);
		if (c == null) {
			assert(flush);
			return -1;
		}
		return c.getId();
	}

	public EquivalenceClass getEquivalenceClass(int i) {
		EquivalenceClass r = classes.get(i-1);
		assert(r.getId() == i);
		return r;
	}

}

