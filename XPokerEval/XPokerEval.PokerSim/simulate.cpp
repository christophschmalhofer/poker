#include "simulate.hpp"
#include "simDefs.hpp"

#include <list>
#include <sstream>

namespace Simulate
{
    uint32 CollaspeCardMask(CardMask cards)
    {
        uint32 ss = CardMask_SPADES(cards);
        uint32 sc = CardMask_CLUBS(cards);
        uint32 sd = CardMask_DIAMONDS(cards);
        uint32 sh = CardMask_HEARTS(cards);

        return (sc | sd | sh | ss);
    }


    void ZeroPostFlopState(PostFlopState* pfs)
    {
        //Zero the structure.
        for (size_t i = 0; i < sizeof(*pfs); ++i)
            ((char*)pfs)[i] = 0;
    }


    unsigned int BoardsToEnumerate(unsigned int cardsToDraw)
    {
        switch (cardsToDraw)
        {
            case 5: return 2118760; //50 choose 5
            case 4: return 211876; //49 choose 4
            case 3: return 17296; //48 choose 3
            case 2: return 1081; //47 choose 2
            case 1: return 46; //46 choose 1
            default: return 0;
        }
    }


    void GetHandState(const Hand& shand, PostFlopState* state)
    {
        ZeroPostFlopState(state);

        if (!CollaspeCardMask(shand.common))
            return; //This shouldn't be called pre flop.

        {
            CardMask first, second;

            CardMask_OR(first, shand.firstHoleCard, shand.common);
            CardMask_OR(second, shand.secondHoleCard, shand.common);

            unsigned int bothRank = Hand_EVAL_N(shand.dead, shand.size);
            unsigned int firstRank = Hand_EVAL_N(first, shand.size - 1);
            unsigned int secondRank = Hand_EVAL_N(second, shand.size - 1);
            unsigned int boardRank = Hand_EVAL_N(shand.common, shand.size - 2);

            if (bothRank > firstRank && bothRank > secondRank)
                state->UsesBoth = true;
            else if (firstRank > boardRank || secondRank > boardRank)
                state->UsesOne = true;
            else
                state->UsesNone = true;

        }

        int currentType = Hand_EVAL_TYPE(shand.dead, shand.size);
        switch (currentType)
        {
            case StdRules_HandType_NOPAIR: state->IsNoPair = true; break;
            case StdRules_HandType_ONEPAIR: state->IsOnePair = true; break;
            case StdRules_HandType_TWOPAIR: state->IsTwoPair = true; break;
            case StdRules_HandType_TRIPS: state->IsTrips = true; break;
            case StdRules_HandType_STRAIGHT: state->IsStraight = true; break;
            case StdRules_HandType_FLUSH: state->IsFlush = true; break;
            case StdRules_HandType_FULLHOUSE: state->IsFullHouse = true; break;
            case StdRules_HandType_QUADS: state->IsQuads = true; break;
            case StdRules_HandType_STFLUSH: state->IsStFlush = true; break;
        }


        if (state->IsNoPair || state->IsOnePair) //Attempt to find out what kind of one pair or nothing.
        {
            uint32 board = CollaspeCardMask(shand.common); //Board cards.
            uint32 h1Rank = CollaspeCardMask(shand.firstHoleCard); //Grab the first card.
            uint32 h2Rank = CollaspeCardMask(shand.secondHoleCard); //second card.


            //Loop through all the board card ranks until we find the highest card.
            uint32 highestBoardCard;
            for (uint32 i = 0x1000; true; i >>= 1) //Start with the 13th bit set for an ace.
                if (board & i)
                {
                    highestBoardCard = i;
                    break;
                }

            //Loop through all the board card ranks until we find the lowest card.
            uint32 lowestBoardCard;
            for (uint32 i = 1; true; i <<= 1) //Start with the 1 bit set for a duece.
                if (board & i)
                {
                    lowestBoardCard = i;
                    break;
                }


            if (h1Rank == highestBoardCard || h2Rank == highestBoardCard)
                state->IsTopPair = true;

            if (h1Rank == lowestBoardCard || h2Rank == lowestBoardCard)
                state->IsBottomPair = true;

            if (h1Rank == h2Rank && h1Rank > highestBoardCard)
                state->IsOverPair = true;
            else if (h1Rank > highestBoardCard && h2Rank > highestBoardCard)
                state->IsOverCards = true;
            else if (state->IsOnePair && !state->IsTopPair && !state->IsBottomPair)
                state->IsMiddlePair = true;
        }


        {//Grab some info about the board itself.
            uint32 ranks = CollaspeCardMask(shand.common);
            uint32 sc, sd, sh, ss;

            ss = CardMask_SPADES(shand.common);
            sh = CardMask_HEARTS(shand.common);
            sd = CardMask_DIAMONDS(shand.common);
            sc = CardMask_CLUBS(shand.common);

            uint32 two_mask = ranks ^ (ss ^ sh ^ sd ^ sc);

            if (two_mask)
                state->IsFullHousePossible = true;
        }

        {//Grab some info about our draws.
            uint32 sc, sd, sh, ss;

            ss = CardMask_SPADES(shand.dead);
            sh = CardMask_HEARTS(shand.dead);
            sd = CardMask_DIAMONDS(shand.dead);
            sc = CardMask_CLUBS(shand.dead);

            if (nBitsTable[ss] == 3 || nBitsTable[sh] == 3 || nBitsTable[sd] == 3 || nBitsTable[sc] == 3)
                state->Is3Flush = true;

            if (nBitsTable[ss] == 4 || nBitsTable[sh] == 4 || nBitsTable[sd] == 4 || nBitsTable[sc] == 4)
                state->Is4Flush = true;

            //Enumerate 1 card, and count our outs for a straight.
            int size = shand.size + 1;
            CardMask next, hand;

            ENUMERATE_N_CARDS_D(next, 1, shand.dead,
            {
                CardMask_OR(hand, next, shand.dead);
                int drawnType = Hand_EVAL_TYPE(hand, size);

                if (drawnType != currentType)
                    switch (drawnType)
                    {
                        case StdRules_HandType_STRAIGHT: ++state->StraightOuts; break;
                        case StdRules_HandType_FLUSH: ++state->FlushOuts; break;

                        case StdRules_HandType_FULLHOUSE:
                        case StdRules_HandType_QUADS:
                        case StdRules_HandType_STFLUSH: ++state->BetterOuts; break;
                    }


                //While we are here, enumerate opponent cards and check for what's possible.
                CardMask totalDead; CardMask_OR(totalDead, hand, next);
                CardMask oppCards;
                ENUMERATE_N_CARDS_D(oppCards, 2, totalDead,
                {
                    //Grab what the opponent could have right now.
                    CardMask oppHand; CardMask_OR(oppHand, oppCards, shand.common);

                    drawnType = Hand_EVAL_TYPE(oppHand, shand.size);
                    if (drawnType == StdRules_HandType_STRAIGHT) state->IsStraightPossible = true;
                    if (drawnType == StdRules_HandType_FLUSH) state->IsFlushPossible = true;

                    //Now consider what the opponent could draw.
                    CardMask_OR(oppHand, oppHand, next);
                    drawnType = Hand_EVAL_TYPE(oppHand, size);
                    if (drawnType == StdRules_HandType_STRAIGHT) state->IsStraightDrawPossible = true;
                    if (drawnType == StdRules_HandType_FLUSH) state->IsFlushDrawPossible = true;

                });
            });
        }
    }


    const char* GetHandStateBrief(const PostFlopState* state)
    {
        static std::ostringstream out;

        out.str("");

        if (state->UsesNone)
            out << "Playing the board, ";
        else if (state->UsesOne)
            out << "Using one hole card, ";
        else if (state->UsesBoth)
            out << "Using both hole cards, ";

        if (state->IsNoPair)
        {
            if (state->IsOverCards)
                out << "with over cards";
            else
                out << "with high card";
        }
        else if (state->IsOnePair)
        {
            if (state->IsTopPair)
                out << "with top pair";
            else if (state->IsMiddlePair)
                out << "with middle pair";
            else if (state->IsBottomPair)
                out << "with bottom pair";
            else if (state->IsOverPair)
                out << "with over pair";
            else
                out << "with one pair";
        }
        else if (state->IsTwoPair)
            out << "with two pair";
        else if (state->IsTrips)
            out << "with trips";
        else if (state->IsStraight)
            out << "with a straight";
        else if (state->IsFlush)
            out << "with a flush";
        else if (state->IsFullHouse)
            out << "with a full house";
        else if (state->IsQuads)
            out << "with quads";
        else if (state->IsStFlush)
            out << "with a straight flush";

        //Do we have a flush and straight draw?
        bool flushDraw = (state->Is3Flush || state->Is4Flush);
        bool straightDraw = (state->StraightOuts);
        bool anotherDraw = ( flushDraw && straightDraw);

        if (anotherDraw)
            out << ", ";
        else
            if (flushDraw || straightDraw)
                out << " and ";

        if (state->Is4Flush)
            out << "4 cards to a flush (" << state->FlushOuts << " outs)";
        else if (state->Is3Flush)
            out << "3 cards to a flush";

        if (anotherDraw)
            out << ", and ";

        if (straightDraw)
            out << state->StraightOuts << " outs to a straight";

        out << ".";

        if (state->BetterOuts)
            out << " " << state->BetterOuts << " outs to boat or better.";

        if (state->IsFlushPossible)
            out << " Someone may have a flush.";
        else if (state->IsFlushDrawPossible)
            out << " Someone may be drawing to a flush.";

        if (state->IsStraightPossible)
            out << " Someone may have a straight.";
        else if (state->IsStraightDrawPossible)
            out << " Someone may be drawing to a straight.";


        if (state->IsFullHousePossible)
            out << " The board is paired.";

        return out.str().c_str();
    }


    unsigned int RankHand(const Hand& shand)
    {
        CardMask e; //Used to hold a hand we are evaluating.

        CardMask_OR(e, shand.ours, shand.common);

        return Hand_EVAL_N(e, shand.size);
    }


    void SimulateHand(const Hand& shand, SimResults* results, float lowRange, float highRange, unsigned int boards)
    {
        SimCurrent(shand, results, lowRange, highRange);
        SimSd(shand, results, lowRange, highRange, boards);
    }


    void SimulateHandMulti(const Hand& shand, SimResults* results, unsigned int boards, unsigned int hands, unsigned int numOpponents)
    {
        //TODO add the current state. Will this really get done? Do I care?
        //SimCurrent(shand, results, lowRange, highRange);
        results->win = 0;
        results->tie = 0;
        results->lose = 0;

        results->evaluations = 0;

        SimSdMulti(shand, results, boards, hands, numOpponents);
    }


    void SimCurrent(const Hand& shand, SimResults* results, float lowRange, float highRange)
    {
        const bool EnumRange = (lowRange != 0 || highRange != 1);

        const CardMask& ours = shand.ours;
        const CardMask& common = shand.common;
        const CardMask& totalDead = shand.dead;

        //Determin the hand size;
        const int& handSize = shand.size;

        #ifdef CountEvaluations
            long int evaluations = 0;
        #endif

        CardMask hand; //Used to hold a hand we are evaluating.

        CardMask_OR(hand, ours, common);
        HandVal ourVal = Hand_EVAL_N_C(hand, handSize); //Save our hands value.

        HandVal theirVal;
        int wins = 0, ties = 0, losses = 0, total = 0;
        CardMask theirs;

        if (EnumRange)
            HostileEnumRange(SimCurrentLoop)
        else
            HostileEnumAll(SimCurrentLoop)

        results->win = float(wins) / total;
        results->tie = float(ties) / total;
        results->lose = float(losses) / total;

        #ifdef CountEvaluations
            results->evaluations = evaluations;
        #endif
    }


    void SimSd(const Hand& shand, SimResults* results, float lowRange, float highRange, size_t uboards)
    {
        const bool EnumRange = (lowRange != 0 || highRange != 1);

        const int boards = uboards; //Note that we need to get this to an int, because that's what poker eval uses.

        const CardMask& ours = shand.ours;
        const CardMask& common = shand.common;
        const CardMask& dead = shand.dead;

        const int handSize = 7;
        const int cardsToDraw = 5 - (shand.size - 2);

        const float d94ToBeat = .94f;
        const float d90ToBeat = .9f;
        const float dNowToBeat = results->win + results->tie;

        #ifdef CountEvaluations
            int evaluations = 0;
        #endif

        CardMask hand; //Temporarily holds either our hand or our opponents.
        HandVal ourVal, theirVal; //Just temporarily holds our hand value vs theirs.

        size_t wins = 0, ties = 0, losses = 0, total = 0; //Out of all showdowns, what happens?
        size_t dNowWins = 0, d94Wins = 0, d90Wins = 0, totalBoards = 0; //Out of all boards, what happens?

        CardMask theirs, commonDraw; //Holds the cards we enumerate.
        CardMask totalDead, totalCommon; //Holds the combination of board cards, and dead cards.

        //NOTE THAT WE ENUMERATE THE BOARDS BEFORE THE OPPONENT CARDS, IF DONE OTHERWISE IT'LL TAKE TWICE THE EVALUATIONS!! (our way we only do our hand once for each board, the other way you must do our hand once for each opponent * each board)
        if (uboards && BoardsToEnumerate(cardsToDraw) > uboards)
        {//MonteCralo for the number of boards.
            if (EnumRange)
                DECK_MONTECARLO_N_CARDS_D(StdDeck, commonDraw, dead, cardsToDraw, boards, {SimSdInnerLoopRange(HostileEnumRange)});
            else
                DECK_MONTECARLO_N_CARDS_D(StdDeck, commonDraw, dead, cardsToDraw, boards, {SimSdInnerLoopRange(HostileEnumAll)});
        }
        else
        {//Enumerate everything.
            if (EnumRange)
                DECK_ENUMERATE_N_CARDS_D(StdDeck, commonDraw, cardsToDraw, dead, {SimSdInnerLoopRange(HostileEnumRange)});
            else
                DECK_ENUMERATE_N_CARDS_D(StdDeck, commonDraw, cardsToDraw, dead, {SimSdInnerLoopRange(HostileEnumAll)});
        }

        results->winSd = float(wins) / total;
        results->tieSd = float(ties) / total;
        results->loseSd = float(losses) / total;

        results->dNow = float(float(dNowWins) / float(totalBoards));
        results->d94 = float(float(d94Wins) / float(totalBoards));
        results->d90 = float(float(d90Wins) / float(totalBoards));

        #ifdef CountEvaluations
            results->evaluations += evaluations; //Add because SimCurrent has already ran.
        #endif
    }


    void SimSdMulti(const Hand& shand, SimResults* results, size_t uboards, size_t hands, size_t numOpponents)
    {
        //Ok, we have our work cut out here.
        //Boards is the number of boards to see.
        //Hands is the number of hands to see for each opponent.

        const int boards = uboards;

        const int oppHands = hands * numOpponents;

        const CardMask& ours = shand.ours;
        const CardMask& common = shand.common;
        const CardMask& dead = shand.dead;

        const int handSize = 7;
        const int cardsToDraw = 5 - (shand.size - 2);

        const float d94ToBeat = .94f;
        const float d90ToBeat = .9f;
        const float dNowToBeat = results->win + results->tie;

        #ifdef CountEvaluations
            long int evaluations = 0;
        #endif

        CardMask hand;

        HandVal ourVal;
        HandVal theirVal;

        size_t wins = 0, ties = 0, losses = 0, total = 0;
        size_t dNowWins = 0, d94Wins = 0, d90Wins = 0, totalBoards = 0;

        CardMask theirs, commonDraw;

        DECK_MONTECARLO_N_CARDS_D(StdDeck, commonDraw, dead, cardsToDraw, boards,
        {

            CardMask totalDead;
            CardMask_OR(totalDead, dead, commonDraw)

            CardMask totalCommon;
            CardMask_OR(totalCommon, common, commonDraw);

            CardMask_OR(hand, ours, totalCommon);
            ourVal = Hand_EVAL_N_C(hand, handSize);

            std::list<HandVal> oppVals;
            std::list<CardMask> oppCards;

            int showDownTotal = 0;
            int showDownWinsTies = 0;

            DECK_MONTECARLO_N_CARDS_D(StdDeck, theirs, totalDead, 2, oppHands,
            {

                CardMask_OR(totalDead, totalDead, theirs);

                CardMask_OR(hand, theirs, totalCommon);
                theirVal = Hand_EVAL_N_C(hand, handSize);

                //Save the enemy's hand.
                oppVals.push_back(theirVal);
                oppCards.push_back(theirs);

                if (oppVals.size() > numOpponents)
                {
                    oppVals.pop_front();

                    CardMask front = oppCards.front();
                    oppCards.pop_front();

                    CardMask_NOT(front, front);
                    CardMask_AND(totalDead, totalDead, front);
                }

                if (oppVals.size() == numOpponents)
                {
                    //Find the strongest enemy.
                    for(std::list<HandVal>::const_iterator it = oppVals.begin(); it != oppVals.end(); ++it)
                    {
                        if ((*it) > theirVal)
                            theirVal = (*it);
                    }

                    ++total;
                    ++showDownTotal;

                    if (ourVal > theirVal)
                        ++wins;
                    else if (ourVal < theirVal)
                        ++losses;
                    else
                        ++ties;

                    if (ourVal >= theirVal)
                        ++showDownWinsTies;
                }
            });

            ++totalBoards;

            if ((float)showDownWinsTies / showDownTotal >= dNowToBeat)
                ++dNowWins;

            if ((float)showDownWinsTies / showDownTotal >= d94ToBeat)
                ++d94Wins;

            if ((float)showDownWinsTies / showDownTotal >= d90ToBeat)
                ++d90Wins;
        });

        results->winSd = float(wins) / total;
        results->tieSd = float(ties) / total;
        results->loseSd = float(losses) / total;

        results->dNow = float(float(dNowWins) / float(totalBoards));
        results->d94 = float(float(d94Wins) / float(totalBoards));
        results->d90 = float(float(d90Wins) / float(totalBoards));

        #ifdef CountEvaluations
            results->evaluations += evaluations; //Add because SimCurrent has already ran.
        #endif
    }


    CardMask MakeCard(const char* s)
    {
            char cs[] = "00";

            std::strcpy(cs, s);

            int c;
            CardMask hand;
            CardMask_RESET(hand);

            StdDeck_stringToCard(cs, &c);
            StdDeck_CardMask_SET(hand, c);

            return hand;
    }
}
