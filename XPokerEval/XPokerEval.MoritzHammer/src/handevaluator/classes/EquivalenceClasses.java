package handevaluator.classes;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.StringTokenizer;

public class EquivalenceClasses {
    public static final int EQUIVALENCE_CLASS_COUNT = 7463;
    private EquivalenceClass[] classes;

    private EquivalenceClasses() {
        try {
            BufferedReader r = new BufferedReader(new FileReader("eqcllist"));
            // 1   AKQJT   8   Royal Flush 100.00  0.0032
            String l;
            classes = new EquivalenceClass[EQUIVALENCE_CLASS_COUNT];   // 1-offset
            int i = 1;
            while ((l = r.readLine()) != null) {
                StringTokenizer st = new StringTokenizer(l, "\t");
                st.nextToken();
                String cards = st.nextToken();
                int gid = Integer.parseInt(st.nextToken());
                String desc = st.nextToken();
                float domination = Float.parseFloat(st.nextToken());
                classes[i] = new EquivalenceClass(i, gid, cards, desc, domination);
                i++;
            }
            System.out.println("Loaded " + i + " classes");
            r.close();
        } catch (IOException e) {
            System.out.println("Cannot load equivalence classes");
            e.printStackTrace();
        }
    }
    
    private static final EquivalenceClasses instance = new EquivalenceClasses();
    
    public static EquivalenceClasses getInstance() {
        return instance;
    }

    public EquivalenceClass getEquivalenceClass(int i) {
        return classes[i];
    }

    public EquivalenceClass getEquivalenceClassForDomination(float percent) {
        EquivalenceClass ret = classes[1];
        for (int i=1; i < classes.length; i++) {
            if (classes[i].getDomination() < percent) {
                return ret;
            }
            ret = classes[i];
        }
        return ret;
    }
    
    
}
