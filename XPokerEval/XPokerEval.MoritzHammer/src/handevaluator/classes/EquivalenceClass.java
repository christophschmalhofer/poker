package handevaluator.classes;

public class EquivalenceClass {
    private int id;
    private int groupid;
    private String cards;
    private String desc;
    private float domination;

    public String toString() {
        return id + " (" + desc + ": " + cards + ")";
    }
    
    public EquivalenceClass(int i, int gid, String cards, String desc, float domination) {
        this.id = i;
        this.groupid = gid;
        this.cards = cards;
        this.desc = desc;
        this.domination = domination;
    }

    public int getId() {
        return id;
    }

    public int getGroupId() {
        return groupid;
    }

    public String getCards() {
        return cards;
    }

    public String getDescription() {
        return desc;
    }

    public float getDomination() {
        return domination;
    }

}
