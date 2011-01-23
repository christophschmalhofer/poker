package PSim;

public class PostFlopState
{
    ///General info about a post flop hand.
    
    ///Do we use both of our hole cards?
    public boolean UsesBoth;
    public boolean UsesOne;
    public boolean UsesNone;

    ///What is our actual hand rank?
    public boolean IsNoPair;
    public boolean IsOnePair;
    public boolean IsTwoPair;
    public boolean IsTrips;
    public boolean IsStraight;
    public boolean IsFlush;
    public boolean IsFullHouse;
    public boolean IsQuads;
    public boolean IsStFlush;

    public boolean IsTopPair; ///<We paired the highest card one the board with one in our hand.
    public boolean IsMiddlePair; ///<We paired the board, but it's not top or bottom pair. (may be 2nd best, 3rd best, etc.)
    public boolean IsBottomPair; ///<We paired the lowest card one the board with one in our hand.
    public boolean IsOverPair; ///<We have a pocket pair, and it's bigger than any card on the board.
    public boolean IsOverCards; ///<We just have two nonpaired cards bigger than anything on the board.

    public boolean IsStraightDrawPossible; ///<Could someone have 4 to a straight?
    public boolean IsStraightPossible; ///Could someone have a straight?
    
    public boolean IsFlushDrawPossible; ///<Could someone have 4 to a suit?
    public boolean IsFlushPossible; ///<Could someone have 5 to a suit?
    
    public boolean IsFullHousePossible; ///<Is a fullhouse or quads possible?

    public boolean Is3Flush; ///<Do we have exactly 3 cards to a flush?
    public boolean Is4Flush; ///<Do we have exactly 4 cards to a flush?

    public int FlushOuts; ///<How many cards will give us flush?
    public int StraightOuts; ///<How many cards will give us straight?
    public int BetterOuts; ///<How many cards will give us a fullhouse or better?
}